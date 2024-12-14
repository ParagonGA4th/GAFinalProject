//�⺻������ Material Path�� ���� ��, �ε�Ǵ� ����Ʈ ���͸��� ���λ�� ���̴� (VS).
//Appends PS �ʼ� ����.

//Append Shader�� ���� ���̴� Commons
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
    
    //Normal�� ���� ������ �� �Ǵµ�.. ���� ����?
    
    float3 Lo = normalize(GetEyePosition() - GetPosition(quadUV));
    float3 N = NormalSampleToWorldSpace(GetNormalMap(quadUV), GetNormal(quadUV), GetTangent(quadUV));
    N = normalize(N);
    
    // ǥ�� �븻 <-> Outgoing �� ���� ������ ��.
    float cosLo = max(0.0, dot(N, Lo)); //NdotV
    
    // ����u�� �ݻ� ����.
    float3 Lr = 2.0 * cosLo * N - Lo;
    
    //�븻 �Ի簢������ ������ �ݻ� ����. (Metal���� Albedo Color�� ����ؾ� �Ѵ�)
    float3 F0 = lerp(Fdielectric, baseColor.xyz, metalness);
    
    //�׽�Ʈ �� �ٸ� �ɷ� ��ü.
    float3 lightDirArr[3] = { normalize(float3(0.3, -0.9, -0.2)), normalize(float3(-0.1, -0.95, -0.15)), normalize(float3(0.25, -0.85, -0.45)) };
    float lightRadianceArr[3] = { firstRad, firstRad, firstRad };
    
    float3 directLighting = 0.0;
    uint tNumLight = 1;
    for (uint i = 0; i < tNumLight; ++i)
    {
        //�ϴ�, SCENEINFORMATIONSENDER -> ���߿� CHANGE.
        float3 Li = -lightDirArr[i];
        float3 Lradiance = lightRadianceArr[i];
      
        //�������� �ý��� ������ ������ �������� �ش簪 ó��.
        //float3 Li = -_dirLightArray[i].direction;
        //float3 Lradiance = _dirLightArray[i].radiance;
  
        //�� �Ի� / �ƿ� ���� ��������
        float3 Lh = normalize(Li + Lo);
  
        //ǥ�� ������ ���� ����Ʈ ���� ������ ���� ���.
        float cosLi = max(0.0, dot(N, Li)); //NdotL
        float cosLh = max(0.0, dot(N, Lh)); //NdotH
  
        // �������� ���� ������ �� ���.
        float3 F = PBR_fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
        
        //Specular BRDF : ���� ���� ���.
        float D = PBR_ndfGGX(cosLh, roughness);
        
        //Specular BRDF : ������ ���� ��� (Attenuation)
        float G = PBR_gaSchlickGGX(cosLi, cosLo, roughness);
  
        // Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
        // Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
        // To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
        //������ ����.
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
  
    //����μ��� IBL ����Ʈ ���.
    float3 ambientLighting = { 0.0f, 0.0f, 0.0f };
    {
         // �븻 ���⿡�� ��ǻ�� Irradiance ���ø�.
         // ...DiffuseHDR.
        float3 irradiance = GetDiffuseIrradianceMap(N);
     
         // Ambient Lighting�� ���� ������ �� ���.
         // �̹� ���͸��Ǿ� �ְ�, Irradiance�� ���� ���⿡�� ���� cosLo�� ����.
        float3 F = PBR_fresnelSchlick(F0, cosLo);
     
         // Diffuse �⿩ ���� �����´� (�������� ������ ���)
        float3 kd = lerp(1.0 - F, 0.0, metalness);
     
         // Irradiance ���� ����Ʈ BRDF ��� ���� Radiance�� ����Ѵ�. 1/PI �ʿ� X.
        float3 diffuseIBL = kd * albedo * irradiance;
     
         // �̸� ���͸��� Specular Reflection ȯ���� �ùٸ� �Ӹ� �������� ���ø�.
         //...SpecularHDR. 
        uint specularTextureLevels = IBL_querySpecularTextureLevels();
        float3 specularIrradiance = GetSpecularIrradianceMap(Lr, roughness * specularTextureLevels);
         // ��-�䷱�� ����ŧ�� BRDF -> ����-�հ� �ٻ�ġ ��� ���ϱ�.
        float2 specularBRDF = IBL_GetSpecularBRDF(float2(cosLo, roughness));
     
         // Specular IBL.
        float ao = GetAmbientOcclusionMap(quadUV);
        float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance * CONTROL_FACTOR;
        specularIBL *= IBL_CONTROL_FACTOR;
        oIndSpecBRDF = specularIBL * ao;
        
      
         // ��ü ������ �⿩ ����.
        ambientLighting = (diffuseIBL + specularIBL) * ao;
    }
   
    //����.
    return float4(directLighting + ambientLighting, 1.0f);
}

//�ݵ�� ��ǲ = VOutQuad, �ƿ�ǲ = POutQuad
POutQuad main(VOutQuad pin)
{
    //��� Appends�� Pixel Shader ���� ����, ClipUnfits �Լ��� ����־�� �Ѵ�. ���� Ư�� ����.
    ClipUnfits(pin.UV);
    
    //�������� Shader Code.
    POutQuad res;
    
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    color = float4(GetAlbedoMap(pin.UV), 1.0f);
    color = pow(color, 2.2f); // Correction.
    
    //�ٽ� Albedo�� ������ ����. �̴� �ӽ�.
    //res.Output = float4(color.xyz, 1.0f);
    
    //����Ʈ���� ���� ���� �� ��Ȳ, �ϴ��� ��������.
    //����Ʈ ���� ���� ���
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
    
    //���������� Saturate�Ǿ ���´�.
    float shadow = ShadowValue(GetPosition(pin.UV), GetNormal(pin.UV), _indep_MainLightDir);
    shadow /= 2.0f;
    res.Output.xyz *= (1.0f - shadow);
    
    return res;
}