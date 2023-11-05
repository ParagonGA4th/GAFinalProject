#include "../Libraries/DefaultLayouts.hlsli"
#include "../Libraries/DefaultBufferPerObject.hlsli"
#include "../Libraries/Lights.hlsli"
#include "../Libraries/misc.hlsli"

Texture2D GBuffer[7];

float4 main(VOutLighting pin) : SV_TARGET
{
	float4 output;
	
	float3 Normal = GBuffer[1].Sample(state, pin.UV).xyz;
	
    for (int i = 0; i < 10; ++i)
    {
        output += directionalLight[i].intensity * (directionalLight[i].ambient + directionalLight[i].diffuse * dot(Normal, directionalLight[i].direction));		
    }
	
	return output;
}