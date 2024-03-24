#ifndef __DEFINED_APPENDS_SCENEINFO_PS_HLSL__
#define __DEFINED_APPENDS_SCENEINFO_PS_HLSL__

//원래는 VS/PS 모두에서 접근할 수 있어야 함. 일단은 구현을 위해 이렇게 설정.

//<Constant Buffers & Functions>
cbuffer cbSceneInfo : register(b4)
{
    float4x4 valViewMatrix;
    float4x4 valProjMatrix;
    float3 valEyePosition;
};

float4x4 GetViewMatrix()
{
    return valViewMatrix;
}

float4x4 GetProjMatrix()
{
    return valProjMatrix;
}

float3 GetEyePosition()
{
    return valEyePosition;
}
//</Constant Buffers & Functions>

//곧 실제로 라이트가 업데이트되겠지만, 일단은 하드코딩된 값으로.

static const uint NumLights = 3;

//<Temp>
static const float3 firstLightDir = { -1.0f, 0.0f, 1.0f };
static const float3 secondLightDir = { 1.0f, 0.0f, 0.0f };
static const float3 thirdLightDir = { 0.0f, -1.0f, 0.0f };

static const float firstRad = 0.1f;
static const float secondRad = 0.1f;
static const float thirdRad = 0.1f;
//</Temp>

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


#endif //__DEFINED_APPENDS_SCENEINFO_PS_HLSL__
