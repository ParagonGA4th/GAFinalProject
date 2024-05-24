//기본적으로 Material Path가 없을 시, 로드되는 디폴트 매터리얼 내부사용 셰이더 (VS).
//Appends PS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"
#include "../../../Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"
#include "../../../Libraries/MathFunctions/Appends_MathFunctions.hlsli"
#include "../../../Libraries/MathFunctions/Appends_ShadowFunctions.hlsli"

float4 DefaultLightingOperation(float2 quadUV)
{
    float3 albedo = sRGB2Lin(GetAlbedoMap(quadUV));
    float metalness = sRGB2Lin(GetMetallicMap(quadUV));
    float roughness = sRGB2Lin(GetRoughnessMap(quadUV));
    
    //아트와 반짝거리는 것 없애기 매핑 중이었음.
     //float roughness = sRGB2Lin(GetRoughnessMap(pin.UV));
    //float roughness = saturate(sRGB2Lin(1.0f - GetRoughnessMap(pin.UV))); //Roughness 문제? 나중에 아트와 빛나는 때깔 맞춰야 한다. 성찬이 형이 말한 "따로"하는 문제일수도.
    //float roughness = saturate(1.0f - GetRoughnessMap(pin.UV));
    //float roughness = 0.0f;
    
    //라이팅 패스가 자리잡기 전까지, 일단 대체용 코드로 셰이더 돌리기.
    //float3 lightDirArr[3] = { firstLightDir, firstLightDir, firstLightDir };
    //float lightRadianceArr[3] = { firstRad, firstRad, firstRad };
    
    //Outgoing 빛의 방향 (WorldPos -> Eye 벡터 방향)
    float3 Lo = normalize(GetEyePosition() - GetPosition(quadUV));
    
    //Normal Mapping. 
    float3 N = NormalSampleToWorldSpace(GetNormalMap(quadUV), GetNormal(quadUV), GetTangent(quadUV));
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
    uint tNumLight = 1;
    for (uint i = 0; i < tNumLight; ++i)
    {
        //float3 Li = -lights[i].direction;
        //float3 Lradiance = lights[i].radiance;
        
        //라이팅이 시스템 상으로 들어오기 전까지는 해당값 처럼.
        float3 Li = -_dirLightArray[i].direction;
        float3 Lradiance = _dirLightArray[i].radiance;
    
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
        ambientLighting = ACES_Filming_Tonemapping(ambientLighting);
    }
   
    
    //리턴.
    return float4(gammaCorrection(directLighting) + ambientLighting, 1.0);
}

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    //모든 Appends의 Pixel Shader 실행 전에, ClipUnfits 함수가 들어있어야 한다. 구조 특성 때문.
    ClipUnfits(pin.UV);
    
    //본격적인 Shader Code.
    POutQuad res;
    
    //라이트맵이 아직 없는 이 상황, 일단은 해제했음.
   //라이트 맵을 쓰는 경우
    if (IsUseLightmap(pin.UV) && gCBuf_IsSceneUseLightmap)
    {
        //이 샘플링되었던 LightmapRGB 값 가져오기 + Gamma Correction.
        float4 lightColor = float4(GetLightmapRGB(pin.UV), 1.f);
        lightColor.rgb = pow(lightColor.rgb, 1.f / 2.2f);
        lightColor.rgb = ACES_Filming_Tonemapping(lightColor.rgb);
        //float4 albedo = float4(sRGB2Lin(GetAlbedoMap(pin.UV)), 1.0f);
        float4 albedo = float4(GetAlbedoMap(pin.UV), 1.0f);
        
        //Color Correction해서 기록.
        res.Output = albedo * float4(lightColor.rgb, 1.0f);
    }
    else
    {
         //라이트맵을 안 쓰는 경우
        res.Output = float4(DefaultLightingOperation(pin.UV));
    }
    
    //이거 아니다. 
    //float shadow = ShadowCalculation(GetPosition(pin.UV), GetNormal(pin.UV), _dirLightArray[0].direction);
    //if (0.9f < shadow)
    //{
    //    res.Output = float4(float3(0.05f, 0.05f, 0.05f), 1.0f);
    //}
    
    
    return res;
}