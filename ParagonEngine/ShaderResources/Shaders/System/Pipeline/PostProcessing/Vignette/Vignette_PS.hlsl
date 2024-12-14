//필요한 요소만 골라서 일차적으로 렌더링 수행.

#include "../../../Libraries/System_QuadLayouts.hlsli"
#include "../../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../../Appends/Libraries/MathFunctions/Appends_MathFunctions.hlsli"
#include "../../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

Texture2D<float4> QuadRTV : register(t5);

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    
    //자체를 ToGBuffer에게 전달할 의도밖에 없다. UV 변환을 두번 거칠 필요가 없다.
    float3 color = QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz;
    
    //같이 Color Correction 역할 역시 수행.    
    //color = CorrectColors(color, pin.UV, 0.1f);
   
    if (gCBuf_IsVignetteOn)
    {
        float2 uv = pin.UV * 2.0 - 1.0;
        float vignette = 1.0 - dot(uv, uv);
        vignette = pow(vignette, 1.5); // 강도 조절을 위해서 Factor 존재.

        // Vignette 값 적용시키기.
        color.rgb *= vignette;
    }
    
    res.Output = float4(color, 1.0f);
    
    return res;
}