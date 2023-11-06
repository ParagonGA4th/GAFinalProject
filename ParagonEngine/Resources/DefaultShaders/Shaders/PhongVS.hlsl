#include "../Libraries/DefaultLayouts.hlsli"
#include "../Libraries/DefaultBufferPerObject.hlsli"

Texture2D GBuffer[7];

VOutLighting main(VinLighting vin)
{
	VOutLighting vout;

	vout.PositionH = float4(vin.PositionW, 1.0f);
	vout.UV = vin.UV;
	
	return vout;
}