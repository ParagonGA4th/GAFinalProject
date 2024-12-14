#ifndef __DEFINED_APPENDS_SCENEINFO_VS_PS_HLSL__
#define __DEFINED_APPENDS_SCENEINFO_VS_PS_HLSL__

//원래는 VS/PS 모두에서 접근할 수 있어야 함. 일단은 구현을 위해 이렇게 설정.
//이제 VB / IB 모두 접근할 수 있게 할 것. 
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
    float4x4 gCBuf_InvViewMatrix;
    
    float2 gCBuf_ScreenWidthHeight;
    float gCBuf_RadianTimeLoop;
    int gCBuf_OnlyShowLMIndex;
    
    float3 gCBuf_EyePosition;
    bool gCBuf_IsSceneUseLightmap; // bool로 캐스팅 상관X.
    bool gCBuf_IsVignetteOn;
    bool gCBuf_IsBloomOn;
    bool gCBuf_IsLutOn;
    bool gCBuf_IsMotionBlurOn;
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
     //Directional Light 기준, 하나 이상 있어야 유효.
    //Single Directional Light Shadow Map을 적용할 터이니.
    float Camera_NearPlane;
    float Camera_FarPlane;
    float tPadding;
    float tPadding2;
    
    
    float4x4 _lightView;
    float4x4 _lightProj;
    float4x4 _lightViewProj;
    
    //Directional Light Array와 독립적으로,
    //셰도우 연산을 위해 첫번째 빛의 LightDir / Radiance는 기록.
    float3 _indep_MainLightDir;
    float _indep_MainLightRadiance;
    
    PgDirectionalLight _dirLightArray[10];
    uint _dirLightCount;
    uint3 _pad1;
    
    PgSpotLight _spotLightArray[10];
    uint _spotLightCount;
    uint3 _pad2;
    
    PgPointLight _pointLightArray[10];
    uint _pointLightCount;
    uint3 _pad3;
}

static const float ShadowBias = 0.005f;

//<Temp>
static const uint NumLights = 1;
static const float3 firstLightDir = { -0.707107, -0.707107, 0.000000 };
static const float firstRad = 1.0f;
//</Temp>

#endif //__DEFINED_APPENDS_SCENEINFO_VS_PS_HLSL__
