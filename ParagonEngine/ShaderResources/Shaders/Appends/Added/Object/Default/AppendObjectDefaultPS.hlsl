//기본적으로 Material Path가 없을 시, 로드되는 디폴트 매터리얼 내부사용 셰이더 (VS).
//Appends PS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"
#include "../../../Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"
#include "../../../Libraries/MathFunctions/Appends_MathFunctions.hlsli"
#include "../../../Libraries/MathFunctions/Appends_ShadowFunctions.hlsli"

float4 DefaultLightingOperation(float4 baseColor, float2 quadUV, out float3 oDirSpecBRDF, out float3 oIndSpecBRDF)
{
    float CONTROL_FACTOR = 1.0f;
    float IBL_CONTROL_FACTOR = 0.3f;
    
    float3 albedo = baseColor.xyz;
    
    float4 color = float4(albedo, 1.0f);
    
    //float metalness = sRGB2Lin(GetMetallicMap(quadUV));
    //float roughness = sRGB2Lin(GetRoughnessMap(quadUV));
    float metalness = GetMetallicMap(quadUV);
    float roughness = GetRoughnessMap(quadUV);
    //float roughness = 1.0f;
    
    //Normal은 방향 유지가 잘 되는데.. 뭐가 문제?
    
    float3 Lo = normalize(GetEyePosition() - GetPosition(quadUV));
    float3 N = NormalSampleToWorldSpace(GetNormalMap(quadUV), GetNormal(quadUV), GetTangent(quadUV));
    N = normalize(N);
    
    // 표면 노말 <-> Outgoing 빛 방향 사이의 각.
    float cosLo = max(0.0, dot(N, Lo)); //NdotV
    
    // 스페큨러 반사 벡터.
    float3 Lr = 2.0 * cosLo * N - Lo;
    
    //노말 입사각에서의 프레넬 반사 정도. (Metal들은 Albedo Color를 사용해야 한다)
    float3 F0 = lerp(Fdielectric, baseColor.xyz, metalness);
    
    //테스트 후 다른 걸로 교체.
    float3 lightDirArr[3] = { normalize(float3(0.3, -0.9, -0.2)), normalize(float3(-0.1, -0.95, -0.15)), normalize(float3(0.25, -0.85, -0.45)) };
    float lightRadianceArr[3] = { firstRad, firstRad, firstRad };
    
    float3 directLighting = 0.0;
    uint tNumLight = 1;
    for (uint i = 0; i < tNumLight; ++i)
    {
        //일단, SCENEINFORMATIONSENDER -> 나중에 CHANGE.
        float3 Li = -lightDirArr[i];
        float3 Lradiance = lightRadianceArr[i];
      
        //라이팅이 시스템 상으로 들어오기 전까지는 해당값 처럼.
        //float3 Li = -_dirLightArray[i].direction;
        //float3 Lradiance = _dirLightArray[i].radiance;
  
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
        specularBRDF *= CONTROL_FACTOR;
        oDirSpecBRDF = specularBRDF;
  
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
        float ao = GetAmbientOcclusionMap(quadUV);
        float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance * CONTROL_FACTOR;
        specularIBL *= IBL_CONTROL_FACTOR;
        oIndSpecBRDF = specularIBL * ao;
        
      
         // 전체 간접광 기여 정도.
        ambientLighting = (diffuseIBL + specularIBL) * ao;
    }
   
    //리턴.
    return float4(directLighting + ambientLighting, 1.0f);
}

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    //모든 Appends의 Pixel Shader 실행 전에, ClipUnfits 함수가 들어있어야 한다. 구조 특성 때문.
    ClipUnfits(pin.UV);
    
    //본격적인 Shader Code.
    POutQuad res;
    
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    color = float4(GetAlbedoMap(pin.UV), 1.0f);
    color = pow(color, 2.2f); // Correction.
    
    //다시 Albedo만 나오게 복귀. 이는 임시.
    //res.Output = float4(color.xyz, 1.0f);
    
    //라이트맵이 아직 없는 이 상황, 일단은 해제했음.
    //라이트 맵을 쓰는 경우
    bool isAlphaClipped = false;
    bool isUseLightmap = IsUseLightmap(pin.UV, isAlphaClipped);
    
    float3 oDirSpecBRDF = 0;
    float3 oIndSpecBRDF = 0;
    float4 col = DefaultLightingOperation(color, pin.UV, oDirSpecBRDF, oIndSpecBRDF);
    
    if (isUseLightmap && gCBuf_IsSceneUseLightmap)
    {
        if (isAlphaClipped)
        {
            res.Output = float4(Uncharted2_Tonemapping(color.xyz), 1.0f);
        }
        else
        {
            float4 lightColor = float4(GetLightmapRGB(pin.UV), 1.f);
            lightColor *= color;
            lightColor = float4(Uncharted2_Tonemapping(lightColor.xyz), 1.0f);
            
            
            oDirSpecBRDF = Uncharted2_Tonemapping(oDirSpecBRDF);
            
            oIndSpecBRDF = TonemappingNormalize(oIndSpecBRDF);
            
            lightColor.xyz += (oDirSpecBRDF + oIndSpecBRDF);

            res.Output = lightColor;
        }
        //res.Output = float4(ACES_Filming_Tonemapping(lightColor.xyz), 1.0f);
        //res.Output = float4(lightColor.xyz, 1.0f);

    }
    else
    {
        res.Output = float4(Uncharted2_Tonemapping(col.xyz), 1.0f);
    }
    
    //내부적으로 Saturate되어서 나온다.
    float shadow = ShadowValue(GetPosition(pin.UV), GetNormal(pin.UV), _indep_MainLightDir);
    shadow /= 2.0f;
    res.Output.xyz *= (1.0f - shadow);
    
    return res;
}