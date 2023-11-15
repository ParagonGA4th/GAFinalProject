#include "../Libraries/DefaultLayouts.hlsli"
#include "../Libraries/Lights.hlsli"
#include "../Libraries/misc.hlsli"

Texture2D GBuffer[7];

float4 main(VOutFinal pin) : SV_TARGET
{
	float4 output = float4(1.0f, 0.0f, 0.0f, 1.0f);
	
	float4 BaseColor = GBuffer[5].Sample(state, pin.UV);
	float4 Phong = GBuffer[6].Sample(state, pin.UV);
	
	//output = BaseColor;
	//output = Phong;
	
	switch(bufferToRender)
	{
		case 0:
			output = GBuffer[0].Sample(state, pin.UV);
			break;
		case 1:
			output = GBuffer[1].Sample(state, pin.UV);
			break;
		case 2:
			output = GBuffer[2].Sample(state, pin.UV);
			break;
		case 3:
			output = GBuffer[3].Sample(state, pin.UV);
			break;
		case 4:
			output = GBuffer[4].Sample(state, pin.UV);
			break;
		case 5:
			output = pow(BaseColor, 1.0 / 2.2);
			break;
		case 6:
			output = GBuffer[6].Sample(state, pin.UV);
			break;
		case 7:
			output = pow(BaseColor * Phong, 1.0 / 2.2);
			break;
		
	}
	
	return output;
}