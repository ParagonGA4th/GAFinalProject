#include "../Libraries/DefaultLayouts.hlsli"
#include "../Libraries/DefaultBufferPerObject.hlsli"
#include "../Libraries/Lights.hlsli"
#include "../Libraries/misc.hlsli"

Texture2D GBuffer[7];

float4 main(VOutLighting pin) : SV_TARGET
{
	float4 output;
	
	float3 Normal = GBuffer[1].Sample(state, pin.UV).xyz;
	
	float intensity;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	
	for(int i = 0; i < 10; ++i)
	{
		// Directional Light
		intensity = directionalLight[i].intensity;
		ambient = directionalLight[i].ambient;
		diffuse = directionalLight[i].diffuse;
		direction = directionalLight[i].direction;
		
		//output += intensity * (ambient + diffuse * dot(Normal, direction));
		output +=  (ambient + diffuse * dot(Normal, direction));
		
	}
	
	output /= 10;
	
	//for(int i = 0; i < 10; ++i)
	//{
	//	// Point Light
	//	intensity = pointLight[i].intensity;
	//	ambient = pointLight[i].ambient;
		
	//	output += (intensity * (ambient));
	//}

	
	return output;
}