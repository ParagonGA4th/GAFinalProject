//�ʿ��� ��Ҹ� ��� ���������� ������ ����.

#include "../../../Libraries/System_QuadLayouts.hlsli"
#include "../../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

Texture2D<float4> QuadRTV : register(t5);

static const float Threshold = 0.8; // Bloom Intensity.
static const float BloomFactor = 1.1; // Bloom �� ���� �̰ɷ�
static const int BlurRadius = 4; // Blur Radius ����.
static const float BlurSigma = 2.0; // Blur �� ���� �̰ɷ�.

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

//�ݵ�� ��ǲ = VOutQuad, �ƿ�ǲ = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    
    //��ü�� ToGBuffer���� ������ �ǵ��ۿ� ����. UV ��ȯ�� �ι� ��ĥ �ʿ䰡 ����.
    float3 color = QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz;
    
    if (gCBuf_IsBloomOn)
    {
        // �̹����� ���� ���� �κ� ����.
        float brightness = max(color.r, max(color.g, color.b));
        float3 bloomColor = step(Threshold, brightness) * color;
    
        // ���� ���� Gaussian Blur ����.
        bloomColor = gaussian_blur(pin.UV, gCBuf_ScreenWidthHeight);
        //bloomColor = gaussian_blur(pin.UV, float2(2560, 1440));

        // ���� ����� Blur ���� ���� Bloom �ϼ�.
        color = color + bloomColor * BloomFactor;
    }
    
    
    res.Output = float4(color, 1.0f);
    return res;
}