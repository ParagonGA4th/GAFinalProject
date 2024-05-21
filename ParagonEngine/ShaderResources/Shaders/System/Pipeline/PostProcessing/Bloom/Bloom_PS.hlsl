//필요한 요소만 골라서 일차적으로 렌더링 수행.

#include "../../../Libraries/System_QuadLayouts.hlsli"
#include "../../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

Texture2D<float4> QuadRTV : register(t5);

static const float Threshold = 0.8; // Bloom Intensity.
static const float BloomFactor = 1.1; // Bloom 힘 조절 이걸로
static const int BlurRadius = 4; // Blur Radius 조절.
static const float BlurSigma = 2.0; // Blur 힘 조절 이걸로.

float4 gaussian_blur(float2 uv, float2 widthHeight)
{
    float4 blurColor = float4(0, 0, 0, 0);
    float2 texelSize = 1.0 / widthHeight;

    for (int y = -BlurRadius; y <= BlurRadius; y++)
    {
        for (int x = -BlurRadius; x <= BlurRadius; x++)
        {
            float2 offset = float2(x, y) * texelSize;
            blurColor += QuadRTV.Sample(fullScreenQuadSS, uv + offset);
        }
    }

    blurColor /= (2 * BlurRadius + 1) * (2 * BlurRadius + 1);

    return blurColor;
}

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    
    //자체를 ToGBuffer에게 전달할 의도밖에 없다. UV 변환을 두번 거칠 필요가 없다.
    float3 color = QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz;
    
    if (gCBuf_IsBloomOn)
    {
        // 이미지의 가장 밝은 부분 추출.
        float brightness = max(color.r, max(color.g, color.b));
        float3 bloomColor = step(Threshold, brightness) * color;
    
        // 밝은 곳에 Gaussian Blur 적용.
        bloomColor = gaussian_blur(pin.UV, gCBuf_ScreenWidthHeight);
        //bloomColor = gaussian_blur(pin.UV, float2(2560, 1440));

        // 원래 색깔과 Blur 색깔 더해 Bloom 완성.
        color = color + bloomColor * BloomFactor;
    }
    
    
    res.Output = float4(color, 1.0f);
    return res;
}