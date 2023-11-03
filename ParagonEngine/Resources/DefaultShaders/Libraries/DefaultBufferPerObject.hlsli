///Constant Buffer들을 셰이더들이 공유해서 사용할 수 있게 세팅하는 HLSL.

#ifndef __DEFINED_DEFAULT_BUFFER_PER_OBJECT_HLSL__
#define __DEFINED_DEFAULT_BUFFER_PER_OBJECT_HLSL__

//Defines
#define PG_MAX_BONECOUNT 100

//Constant Buffers
cbuffer cbPerObjectBase : register(b0)
{
    float4x4 gCBuf_World;
    float4x4 gCBuf_WorldInvTranspose;
    float4x4 gCBuf_WorldViewProj;
    float3 gCBuf_CameraPositionW;
    // gCBuf_Materials[10] //후에 Material이 들어가면 추가되어야.
};

cbuffer cbPerObjectSkinned
{
    float4x4 gCBuf_Bones[PG_MAX_BONECOUNT];
};

// TODO: 라이트를 구조체로 만들고 구조체 배열을 상수버퍼로 받는다
cbuffer cbDirectionalLight
{

	float4 Directional_color;

	float4 Directional_ambient;
	float4 Directional_diffuse;
	float4 Directional_specular;

	float Directional_intensity;
	
	float3 Directional_direction;
};

cbuffer cbPointLight
{
	float4 Point_color;

	float4 Point_ambient;
	float4 Point_diffuse;
	float4 Point_specular;

	float Point_intensity;
	
	float3 Point_attenuation;
	float Point_range;
};

//cbuffer cbSpotLight
//{
//	float4 Spot_color;

//	float4 Spot_ambient;
//	float4 Spot_diffuse;
//	float4 Spot_specular;

//	float Spot_intensity;
	
//	float3 Spot_attenuation;
//	float Spot_range;
//};
#endif //__DEFINED_DEFAULT_BUFFER_PER_OBJECT_HLSL__
