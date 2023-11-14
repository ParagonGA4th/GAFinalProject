#include "../Libraries/DefaultLayouts.hlsli"
#include "../Libraries/Lights.hlsli"
#include "../Libraries/misc.hlsli"

Texture2D GBuffer[7];

float4 main(VOutLighting pin) : SV_TARGET
{
    float4 output = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	float3 Normal = normalize(GBuffer[1].Sample(state, pin.UV).xyz);
	float3 PositionW = GBuffer[2].Sample(state, pin.UV).xyz;

	float3 Light;
	float3 Reflect;
	float3 View = normalize(camPosW - PositionW);
	
	float intensity;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	
	
	// Directional Light
	for(int i = 0; i < 10; ++i)
	{
		Light = directionalLight[i].direction;
	
		direction = -Light;
		intensity = directionalLight[i].intensity;
		ambient = directionalLight[i].ambient;
		diffuse = directionalLight[i].diffuse * max(dot(Normal, direction), 0);
		specular = directionalLight[i].specular * pow(max(dot(reflect(-direction, Normal), View), 0), 8);
		
		output += intensity * (ambient + diffuse + specular);
	}
	
	
	//Point Light
	float d;
	float a2;
	float a1;
	float a;
	
	for(int i = 0; i < 10; ++i)
	{
		Light = normalize(pointLight[i].position.xyz - PositionW);
		
		direction = Light;
		intensity = pointLight[i].intensity;
		ambient = pointLight[i].ambient;
		diffuse = pointLight[i].diffuse * max(dot(Normal, direction), 0);
		specular = pointLight[i].specular * pow(max(dot(reflect(Light, Normal), View), 0), 8);
		
		d = length(PositionW - pointLight[i].position.xyz);
		a2 = pointLight[i].attenuation.x;
		a1 = pointLight[i].attenuation.y;
		a = pointLight[i].attenuation.z;
		
		output += intensity * (ambient + (diffuse + specular / ((a2 * d * d) + (a1 * d) + a + 1)));
	}
	
	
	// Spot Light
	float ks;
	
	for(int i = 0; i < 10; ++i)
	{
		Light = normalize(PositionW - spotLight[i].position.xyz);
		
		direction = normalize(-spotLight[i].direction);
		intensity = spotLight[i].intensity;
		ambient = spotLight[i].ambient;
		diffuse = spotLight[i].diffuse * max(dot(Normal, -Light), 0);
		specular = spotLight[i].specular * pow(max(dot(reflect(Light, Normal), View), 0), 8);
		
		d = length(PositionW - spotLight[i].position.xyz);
		a2 = spotLight[i].attenuation.x;
		a1 = spotLight[i].attenuation.y;
		a = spotLight[i].attenuation.z;
		
		ks = pow(max(dot(direction, -Light), 0), 32);
		
		output += intensity * ks * (ambient + (diffuse + specular / ((a2 * d * d) + (a1 * d) + a + 1) ) );
	}

	return output;
}