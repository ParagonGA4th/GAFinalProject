#include "../Libraries/DefaultLayouts.hlsli"
#include "../Libraries/misc.hlsli"

Texture2D GBuffer[7];

float4 main(VOutFinal pin) : SV_TARGET
{
	float4 output = float4(1.0f, 0.0f, 0.0f, 1.0f);
	
	float4 BaseColor = GBuffer[5].Sample(state, pin.UV);
	float4 Phong = GBuffer[6].Sample(state, pin.UV);
	
	//output = Phong;
	output = BaseColor * Phong;
	
	return output;
}