//필요한 요소만 골라서 일차적으로 렌더링 수행.

#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    //
    //float4 tAmbientColor = { 0.15f, 0.15f, 0.15f, 1.0f };
    //float4 tDiffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    //
    ////Bias 값 설정.
    //float tBias = 0.001f;
    //
    //float3 outColor = tAmbientColor;
    //
    //
   ////res.Output = ;
    //
    return res;
}