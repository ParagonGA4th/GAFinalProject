//필요한 요소만 골라서 일차적으로 렌더링 수행.

#include "../../../Libraries/System_QuadLayouts.hlsli"
#include "../../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"

Texture2D<float4> QuadRTV : register(t5);

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    
    //자체를 ToGBuffer에게 전달할 의도밖에 없다. UV 변환을 두번 거칠 필요가 없다.
    //이거 이제 개별적으로 적용 => 다른 용도로 사용해야!
    float3 val = QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz;
    
    //<ACES Filmic Tone Mapping Curve>
   //float a = 2.51f;
   //float b = 0.03f;
   //float c = 2.43f;
   //float d = 0.59f;
   //float e = 0.14f;
    
    // float3 val = gammaCorrection(saturate((x * (a * x + b)) / (x * (c * x + d) + e)));
    //float3 val = saturate((x * (a * x + b)) / (x * (c * x + d) + e));
    //인터넷에는 -> gammaCorrection을 했었지만, 난 이미 gammaCorrection 된 거에다 적용한 거니까
    //굳이 필요 없을 것 같다!! => 김용준 교수님 PPT 참고.
     //</ACES Filmic Tone Mapping Curve>
    
    //더 이상 Tonemapping은 쓰지 않는다.
    //개별적인 처리가 될 것이기에.
    
    res.Output = float4(val, 1.0f);
    
    return res;
}