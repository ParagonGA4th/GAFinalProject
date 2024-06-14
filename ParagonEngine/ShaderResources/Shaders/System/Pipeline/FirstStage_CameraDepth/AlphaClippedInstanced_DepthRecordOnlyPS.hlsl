///그래픽엔진 자체의 1st Pixel Shader (All)
#pragma pixel_shader main
#pragma target 5.0

#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"

Texture2DArray<float4> AlphaTextureArray : register(t9);
const static float ALPHA_CLIPPING_THRESHOLD = 0.5f;

POutQuad main(VOut1st_Instanced input)
{
    POutQuad output;
    
    float3 tT2UV3 = float3(input.vout1st_Tex, input.vout1st_MeshMatID);
    clip(AlphaTextureArray.Sample(defaultTextureSS, tT2UV3) - ALPHA_CLIPPING_THRESHOLD);
    
    //float linearDepth = input.vout1st_PosH.w / input.vout1st_PosH.z;
    float linearDepth = input.vout1st_PosH.z / input.vout1st_PosH.w;
    output.Output = float4(linearDepth, linearDepth, linearDepth, 1.f);
    
    return output;
}

