#include "../Libraries/DefaultLayouts.hlsli"
#include "../Libraries/DefaultBufferPerObject.hlsli"
#include "../Libraries/misc.hlsli"

Texture2D GBuffer[7];

float4 main(VOutLighting pin) : SV_TARGET
{
	float4 output;
	
	float3 Normal = GBuffer[1].Sample(state, pin.UV).xyz;
	
	output = Directional_intensity * (Directional_ambient + Directional_diffuse * dot(Normal, Directional_direction));
	
	return output;
}