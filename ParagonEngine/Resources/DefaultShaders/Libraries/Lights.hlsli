struct DirectionalLight
{
	float4 position;

    float4 color;

    float4 ambient;
    float4 diffuse;
    float4 specular;

    float intensity;
	
    float3 direction;
};

struct PointLight
{
	float4 position;
	
    float4 color;

    float4 ambient;
    float4 diffuse;
    float4 specular;

    float intensity;
    
    float3 attenuation;
    float range;
};

struct SpotLight
{
	float4 position;
	
    float4 color;

    float4 ambient;
    float4 diffuse;
    float4 specular;

    float intensity;
    
	float3 direction;
	
    float3 attenuation;
    float range;
};

cbuffer cbDirectionalLight : register(b0)
{
	DirectionalLight directionalLight[10];
};

cbuffer cbPointLight : register(b1)
{
    PointLight pointLight[10];
};

cbuffer cbSpotLight : register(b2)
{
    SpotLight spotLight[10];
};

cbuffer cbLightingData : register(b3)
{
	float3 camPosW;
	unsigned int bufferToRender;
	// 0 - 6 : GBuffers
	// 7 : MainRT
}