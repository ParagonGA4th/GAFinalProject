//필요한 요소만 골라서 일차적으로 렌더링 수행.

#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"

static const float Weight[13] =
{
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231,
    1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

//static const float Total = 6.2108;
static const float Total = 20.2108;

Texture2D<float4> OutlineGBufferSRV : register(t7);

cbuffer cbScreenWidthHeight : register(b7)
{
    //Blur를 위해, Screen Width / Height을 차례로 기록.
    float2 widthHeight;
};

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    int blurRadius = 5;

    float2 texelSize = 1.0 / widthHeight;

    // Horizontal Blur
    float4 horizontalBlur = 0;
    for (int i = -blurRadius; i <= blurRadius; ++i)
    {
        horizontalBlur += OutlineGBufferSRV.Sample(fullScreenQuadSS, pin.UV + float2(i, 0) * texelSize);
    }
    horizontalBlur /= (2.0 * blurRadius + 1.0);

    // Vertical Blur
    float4 finalColor = 0;
    for (int j = -blurRadius; j <= blurRadius; ++j)
    {
        finalColor += OutlineGBufferSRV.Sample(fullScreenQuadSS, pin.UV + float2(0, j) * texelSize);
    }
    finalColor /= (2.0 * blurRadius + 1.0);
    
    finalColor.r = ceil(finalColor.r);
    finalColor.g = ceil(finalColor.g);
    
    //알파값이 0 미만이면 자르기.
    float threshold = 1e-6f;
    clip(finalColor.a - threshold);
    
    POutQuad res;
    res.Output = finalColor;
    
    return res;
}