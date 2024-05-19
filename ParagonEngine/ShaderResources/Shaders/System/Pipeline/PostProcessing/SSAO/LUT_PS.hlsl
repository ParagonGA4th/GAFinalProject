//필요한 요소만 골라서 일차적으로 렌더링 수행.

#include "../../../Libraries/System_QuadLayouts.hlsli"
#include "../../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../../Appends/Libraries/SceneInfo/Appends_SceneInfoPS.hlsli"
#include "../../../../Appends/Libraries/TextureBuffers/Appends_GBufferTextures.hlsli"

Texture2D<float4> QuadRTV : register(t5);

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    float3 val = QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz;
    res.Output = float4(val, 1.0f);
   
    return res;
}