//SystemQuad에서 MainRenderTarget으로 뿌려주는 역할. (PS)
#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../Appends/Libraries/MathFunctions/Appends_MathFunctions.hlsli"

Texture2D<float4> QuadRTV : register(t5);

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    //자체를 MainRenderTarget에게 전달할 의도밖에 없다. UV 변환을 두번 거칠 필요가 없다.
    float3 x = QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz;
    
    //<ACES Filmic Tone Mapping Curve>
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    
    float3 val = gammaCorrection(saturate((x * (a * x + b)) / (x * (c * x + d) + e)));
    //</ACES Filmic Tone Mapping Curve>
    res.Output = float4(val, 1.0f);
    
    return res;
}