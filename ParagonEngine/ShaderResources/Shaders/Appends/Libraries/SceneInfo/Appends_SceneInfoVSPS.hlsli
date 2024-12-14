#ifndef __DEFINED_APPENDS_SCENEINFO_VS_PS_HLSL__
#define __DEFINED_APPENDS_SCENEINFO_VS_PS_HLSL__

//������ VS/PS ��ο��� ������ �� �־�� ��. �ϴ��� ������ ���� �̷��� ����.
//���� VB / IB ��� ������ �� �ְ� �� ��. 
//<ActualLighting>
struct PgDirectionalLight
{
    //8Byte
    float3 color;
    float radiance;
    float3 direction;
    float tTempPadding; // ���� �ȵ�.
};

struct PgPointLight
{
    //8Byte, declspec(align(16)) �ڵ�.
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
    float tTempPadding; // ���� �ȵ�.
};

//</ActualLighting>

//<Constant Buffers & Functions>
cbuffer cbSceneInfo : register(b4) // �� ��ü�� VS / PS ��� ���� ����.
{
    float4x4 gCBuf_ViewMatrix;
    float4x4 gCBuf_ProjMatrix;
    float4x4 gCBuf_ViewProjMatrix;
    float4x4 gCBuf_InvViewMatrix;
    
    float2 gCBuf_ScreenWidthHeight;
    float gCBuf_RadianTimeLoop;
    int gCBuf_OnlyShowLMIndex;
    
    float3 gCBuf_EyePosition;
    bool gCBuf_IsSceneUseLightmap; // bool�� ĳ���� ���X.
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

//�� ������ ����Ʈ�� ������Ʈ�ǰ�����, �ϴ��� �ϵ��ڵ��� ������.

cbuffer cbRenderingInfo : register(b5)
{
     //Directional Light ����, �ϳ� �̻� �־�� ��ȿ.
    //Single Directional Light Shadow Map�� ������ ���̴�.
    float Camera_NearPlane;
    float Camera_FarPlane;
    float tPadding;
    float tPadding2;
    
    
    float4x4 _lightView;
    float4x4 _lightProj;
    float4x4 _lightViewProj;
    
    //Directional Light Array�� ����������,
    //�ε��� ������ ���� ù��° ���� LightDir / Radiance�� ���.
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
