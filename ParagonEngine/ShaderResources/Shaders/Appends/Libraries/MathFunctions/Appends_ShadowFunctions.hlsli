
#ifndef __DEFINED_APPENDS_SHADOW_FUNCTIONS_PS_HLSL__
#define __DEFINED_APPENDS_SHADOW_FUNCTIONS_PS_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"
#include "../SceneInfo/Appends_SceneInfoVSPS.hlsli"

Texture2D<float> GlobalShadowDepth_DSV_MainLight : register(t23); // DSV를 SRV로 접근한 결과. - Light
Texture2D<float> GlobalShadowDepth_DSV_Camera : register(t24); // DSV를 SRV로 접근한 결과. - Camera

const static uint SIZED_UP_SHADOW_VP_WIDTH = 4096;
const static uint SIZED_UP_SHADOW_VP_HEIGHT = 4096;

//GetDepth -> DSV에서 옴
float GetLightDepth_DSV(float2 pinUV)
{
    return GlobalShadowDepth_DSV_MainLight.Sample(fullScreenQuadSS, pinUV).r;
}

float GetCameraDepth_DSV(float2 pinUV)
{
    return GlobalShadowDepth_DSV_Camera.Sample(fullScreenQuadSS, pinUV).r;
}

float ShadowCalculation(float3 worldPos, float3 normal, float3 lightDir)
{
    float4 fragPosLightSpace = mul(float4(worldPos, 1.0f), _lightViewProj);
    
    float4 projCoords = fragPosLightSpace * float4(0.5f, -0.5f, 1.0f, 1.0f) + float4(0.5f, 0.5f, 0.0f, 1.0f);
    
    float closestDepth = GlobalShadowDepth_DSV_MainLight.Sample(defaultTextureSS, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), ShadowBias);
    //float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    float shadow = 0.0;
    float2 texelSize = 1.0 / float2(10, 10); //Shadow의 Map Size가 10, 10으로 처리되었었다.
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = GlobalShadowDepth_DSV_MainLight.Sample(defaultTextureSS, projCoords.xy + float2(x, y) * texelSize).r;
            //float pcfDepth = texture(shadowMap, projCoords.xy + float2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0f; // 3x3을 돌렸기에 9로 나눔.
    
    return shadow;
}

#endif // __DEFINED_APPENDS_SHADOW_FUNCTIONS_PS_HLSL__