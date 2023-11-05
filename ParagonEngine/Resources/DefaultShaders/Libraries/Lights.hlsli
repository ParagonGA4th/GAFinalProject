struct DirectionalLight
{
    float4 color;

    float4 ambient;
    float4 diffuse;
    float4 specular;

    float intensity;
	
    float3 direction;
};

struct PointLight
{
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
    float4 color;

    float4 ambient;
    float4 diffuse;
    float4 specular;

    float intensity;
    
    float3 attenuation;
    float range;
};

cbuffer cbLight
{
    DirectionalLight directionalLight[10];
    PointLight pointLight[10];
    SpotLight spotLight[10];
    
};