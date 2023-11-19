#include "../Libraries/DefaultLayouts.hlsli"
#include "../Libraries/DefaultBufferPerObject.hlsli"

Texture2D GBuffer[7];

VOutFinal main(VInFinal vin)
{
	VOutFinal vout;

	vout.PositionH = float4(vin.Position, 1.0f);
	vout.UV = vin.UV;
	
	return vout;
}