#include "../Libraries/DefaultLayouts.hlsli"
#include "../Libraries/Lights.hlsli"
#include "../Libraries/misc.hlsli"

Texture2D GBuffer[7];

float4 main(VOutLighting pin) : SV_TARGET
{
    float4 output = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	float3 Normal = GBuffer[1].Sample(state, pin.UV).xyz;
	
	float intensity;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	
	// Directional Light
    for (int i = 0; i < 10; ++i)
    {
        intensity = directionalLight[i].intensity;
        ambient = directionalLight[i].ambient;
        diffuse = directionalLight[i].diffuse;
        specular = directionalLight[i].specular;
        direction = directionalLight[i].direction;
		
		// specular - r dot v. view vector, reflection vector 
		
        output += intensity * (ambient + diffuse * dot(Normal, direction) + specular * 1.0f);
    }
	
	// Point Light
  //  for (int i = 0; i < 10; ++i)
  //  {
		//// light vector ±¸ÇÏ±â + n dot l
		//// cameraPosition - PositionW

  //      intensity = pointLight[i].intensity;
  //      ambient = pointLight[i].ambient;
  //      diffuse = pointLight[i].diffuse;
  //      specular = pointLight[i].specular;
		
  //      output += intensity * (ambient + diffuse * 0.5f + specular * 0.5f);
  //  }
	
	//// Spot Light
 //   for (int i = 0; i < 10; ++i)
 //   {
 //       intensity = pointLight[i].intensity;
 //       ambient = pointLight[i].ambient;
		
 //       output += intensity * (ambient);
 //   }

	return output;
}