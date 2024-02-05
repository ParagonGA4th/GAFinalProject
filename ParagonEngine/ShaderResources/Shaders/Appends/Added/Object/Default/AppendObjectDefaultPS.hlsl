//기본적으로 Material Path가 없을 시, 로드되는 디폴트 매터리얼 내부사용 셰이더 (VS).
//Appends PS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"
#include "../../../Libraries/SceneInfo/Appends_SceneInfoPS.hlsli"
#include "../../../Libraries/MathFunctions/Appends_MathFunctions.hlsli"

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    //모든 Appends의 Pixel Shader 실행 전에, ClipUnfits 함수가 들어있어야 한다. 구조 특성 때문.
    ClipUnfits(pin.UV);
    
    //본격적인 Shader Code.
    POutQuad res;
    
    //PBR 적용.
    float3 albedo = GetAlbedoMap(pin.UV);
    float metalness = GetMetallicMap(pin.UV);
    float roughness = GetRoughnessMap(pin.UV);
    
    //라이팅 패스가 자리잡기 전까지, 일단 대체용 코드로 셰이더 돌리기.
    float3 lightDirArr[3] = { firstLightDir, secondLightDir, thirdLightDir };
    float lightRadianceArr[3] = { firstRad, secondRad, secondRad };
    
;   //Outgoing 빛의 방향 (WorldPos -> Eye 벡터 방향)
    float3 Lo = normalize(GetEyePosition() - GetPosition(pin.UV));
    
    //Normal Mapping. 
    float3 N = NormalSampleToWorldSpace(GetNormalMap(pin.UV), GetNormal(pin.UV), GetTangent(pin.UV));
    N = normalize(N);
    //float3 N = GetNormal(pin.UV);
    
    // 표면 노말 <-> Outgoing 빛 방향 사이의 각.
    float cosLo = max(0.0, dot(N, Lo)); //NdotV
    
    // 스페큨러 반사 벡터.
    float3 Lr = 2.0 * cosLo * N - Lo;
    
    //노말 입사각에서의 프레넬 반사 정도. (Metal들은 Albedo Color를 사용해야 한다)
    float3 F0 = lerp(Fdielectric, albedo, metalness);
    
    // 위치/빛 정보가 있는 라이팅을 위한 직접과 연산.
    float3 directLighting = 0.0;
    for (uint i = 0; i < NumLights; ++i)
    {
        //float3 Li = -lights[i].direction;
        //float3 Lradiance = lights[i].radiance;
        
        //라이팅이 시스템 상으로 들어오기 전까지는 해당값 처럼.
        float3 Li = -lightDirArr[i];
        float3 Lradiance = lightRadianceArr[i];
    
        //빛 입사 / 아웃 사이 하프벡터
        float3 Lh = normalize(Li + Lo); 
    
        //표면 법선과 여러 라이트 벡터 사이의 각도 계산.
        float cosLi = max(0.0, dot(N, Li)); //NdotL
        float cosLh = max(0.0, dot(N, Lh)); //NdotH
    
        // 직접광을 위한 프레넬 값 계산.
        float3 F = PBR_fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
        //Specular BRDF : 법선 분포 계산.
        float D = PBR_ndfGGX(cosLh, roughness);
		//Specular BRDF : 기하적 감쇠 계산 (Attenuation)
        float G = PBR_gaSchlickGGX(cosLi, cosLo, roughness);
    
		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
        //에너지 보존.
        float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);
    
		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
        float3 diffuseBRDF = kd * albedo;
    
		// Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);
    
		// Total contribution for this light.
        directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
    }
    
    //현재로서는 IBL 디폴트 사용.
    float3 ambientLighting = { 0.0f, 0.0f, 0.0f };
    {
        // 노말 방향에서 디퓨즈 Irradiance 샘플링.
	    // ...DiffuseHDR.
        float3 irradiance = GetDiffuseIrradianceMap(N);
    
	    // Ambient Lighting을 위한 프레넬 텀 계산.
	    // 이미 필터링되어 있고, Irradiance가 여러 방향에서 오니 cosLo를 쓴다.
        float3 F = PBR_fresnelSchlick(F0, cosLo);
    
	    // Diffuse 기여 팩터 가져온다 (직접광과 동일한 방식)
        float3 kd = lerp(1.0 - F, 0.0, metalness);
    
	    // Irradiance 맵은 램버트 BRDF 기반 방출 Radiance를 기록한다. 1/PI 필요 X.
        float3 diffuseIBL = kd * albedo * irradiance;
    
	    // 미리 필터링된 Specular Reflection 환경을 올바른 밉맵 레벨에서 샘플링.
	    //...SpecularHDR. 
        uint specularTextureLevels = IBL_querySpecularTextureLevels();
        float3 specularIrradiance = GetSpecularIrradianceMap(Lr, roughness * specularTextureLevels);
    
		// 쿡-토런스 스페큘러 BRDF -> 분할-합계 근사치 계수 구하기.
        float2 specularBRDF = IBL_GetSpecularBRDF(float2(cosLo, roughness));
    
		// Specular IBL.
        float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
    
		// 전체 간접광 기여 정도.
        ambientLighting = diffuseIBL + specularIBL;
    }
   
    //리턴.
    res.Output = float4(directLighting + ambientLighting, 1.0);
    
    //GammaCorrection까지!
    //res.Output = pow(res.Output, 1.0 / 2.2);
    
    return res;
}