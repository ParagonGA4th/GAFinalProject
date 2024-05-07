#ifndef __DEFINED_APPENDS_SCENEINFO_PS_HLSL__
#define __DEFINED_APPENDS_SCENEINFO_PS_HLSL__

//원래는 VS/PS 모두에서 접근할 수 있어야 함. 일단은 구현을 위해 이렇게 설정.

//<ActualLighting>
struct PgDirectionalLight
{
    //8Byte
    float3 color;
    float radiance;
    float3 direction;
    float tTempPadding; // 쓰면 안됨.
};

struct PgPointLight
{
    //8Byte, declspec(align(16)) 자동.
    float3 color;
    float radiance;
    float range;
    float3 attenuation;
};

struct PgSpotLight
{
    //12Byte
    float3 color;
    float radiance;
    float3 direction;
    float range;
    float3 attenuation;
    float tTempPadding; // 쓰면 안됨.
};

//</ActualLighting>

//<Constant Buffers & Functions>
cbuffer cbSceneInfo : register(b4) // 이 자체는 VS / PS 모두 접근 가능.
{
    float4x4 gCBuf_ViewMatrix;
    float4x4 gCBuf_ProjMatrix;
    float4x4 gCBuf_ViewProjMatrix;
    
    float3 gCBuf_EyePosition;
};

float4x4 GetViewMatrix()
{
    return gCBuf_ViewMatrix;
}

float4x4 GetProjMatrix()
{
    return gCBuf_ProjMatrix;
}

float4x4 GetViewProjMatrix()
{
    return gCBuf_ViewProjMatrix;
}

float3 GetEyePosition()
{
    return gCBuf_EyePosition;
}
//</Constant Buffers & Functions>

//곧 실제로 라이트가 업데이트되겠지만, 일단은 하드코딩된 값으로.

cbuffer cbRenderingInfo : register(b5)
{
    PgDirectionalLight _dirLightArray[10];
    uint _dirLightCount;
    
    PgSpotLight _spotLightArray[10];
    uint _spotLightCount;
    
    PgPointLight _pointLightArray[10];
    uint _pointLightCount;
    
    //Directional Light 기준, 하나 이상 있어야 유효.
    //Single Directional Light Shadow Map을 적용할 터이니.
    float4x4 _lightView;
    float4x4 _lightProj;
    float4x4 _lightViewProj;
}

static const float ShadowBias = 0.001f;

//<Temp>
static const uint NumLights = 1;
static const float3 firstLightDir = { 0, -1, 0 };
static const float firstRad = 0.1f;
//</Temp>

#endif //__DEFINED_APPENDS_SCENEINFO_PS_HLSL__
