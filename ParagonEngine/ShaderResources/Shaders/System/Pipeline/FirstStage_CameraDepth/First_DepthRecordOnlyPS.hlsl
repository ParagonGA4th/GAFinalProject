///그래픽엔진 자체의 1st Pixel Shader (All)
#pragma pixel_shader main
#pragma target 5.0

#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../Libraries/System_QuadLayouts.hlsli"

POutQuad main(VOut1st input)
{
    POutQuad output;
	
    float linearDepth = input.vout1st_PosH.w / input.vout1st_PosH.z;
    output.Output = float4(linearDepth, linearDepth, linearDepth, 1.f);
    
    return output;
}

