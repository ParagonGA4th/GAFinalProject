//필요한 요소만 골라서 일차적으로 렌더링 수행.

#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"

//X/Y Blur를 위해서, 커널 메모리를 바인딩해놓는다.
static const float KernelOffsets[3] = { 0.0f, 1.3846153846f, 3.2307692308f };
static const float BlurWeights[3] = { 0.2270270270f, 0.3162162162f, 0.0702702703f };

Texture2D<float4> OutlineGBufferSRV : register(t7);

cbuffer cbScreenWidthHeight : register(b7)
{
    //Blur를 위해, Screen Width / Height을 차례로 기록.
    float2 widthHeight;
};

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    
    //Gaussian을 쓰는 것은 아니다, Outline 목적이기에, 덜 비싼 값을 사용해도 문제 X!
    //X-Y Blur.
    
   //원래 있던 블러 코드.
   float3 textureColor = float3(1.0f, 0.0f, 0.0f);
   float2 uv = pin.UV;
   
   textureColor = OutlineGBufferSRV.Sample(fullScreenQuadSS, uv).xyz * BlurWeights[0];
   
   //Horizontal Blur.
   [unroll]
   for (int i = 1; i < 3; i++)
   {
       float2 normalizedOffset = float2(KernelOffsets[i], 0.0f) / widthHeight.x;
       textureColor += OutlineGBufferSRV.Sample(fullScreenQuadSS, saturate(uv + normalizedOffset)).xyz * BlurWeights[i];
       textureColor += OutlineGBufferSRV.Sample(fullScreenQuadSS, saturate(uv - normalizedOffset)).xyz * BlurWeights[i];
   }
   
   //Vertical Blur.
   [unroll]
   for (int j = 1; j < 3; j++)
   {
       float2 normalizedOffset = float2(0.0f, KernelOffsets[j]) / widthHeight.y;
       textureColor += OutlineGBufferSRV.Sample(fullScreenQuadSS, saturate(uv + normalizedOffset)).xyz * BlurWeights[j];
       textureColor += OutlineGBufferSRV.Sample(fullScreenQuadSS, saturate(uv - normalizedOffset)).xyz * BlurWeights[j];
   }
   
    res.Output = float4(textureColor, 1.f);
    
    
    //res.Output = OutlineGBufferSRV.Sample(fullScreenQuadSS, pin.UV);
    
    return res;
}