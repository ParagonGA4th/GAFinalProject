//�ʿ��� ��Ҹ� ��� ���������� ������ ����.

#include "../../../Libraries/System_QuadLayouts.hlsli"
#include "../../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"
#include "../../../../Appends/Libraries/TextureBuffers/Appends_GBufferTextures.hlsli"
#include "../../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

Texture2D<float4> QuadRTV : register(t5);

Texture2D<float4> LookUpTable : register(t1);

//LUT�� �ִ� ���� ����.
const static float COLORS = 32.0f;
const static float _Contribution = 0.2f; // 0-1 ����. ���� �����̴�.

//����Ƽ ��������, NameHack�� �ִ�.
//float4 _LUT_TexelSize -> NNN_TexelSize�� ������, z�� NNN�� width, w�� NNN�� height�� ��ȯ.
// �ᱹ �ؽ����� Size���� �޴� �ڵ�.
cbuffer cbLutWidthHeight : register(b0)
{
    //x�� y�� ���δ�.
    float4 lutWidthHeight;
};

float3 GetLUTColor(float2 inputUV, float4 color)
{
  //float maxColor = COLORS - 1.0;
  //float4 col = saturate(QuadRTV.Sample(fullScreenQuadSS, inputUV));
  //float halfColX = 0.5 / lutWidthHeight.x;
  //float halfColY = 0.5 / lutWidthHeight.y;
  //float threshold = maxColor / COLORS;
  //
  //float xOffset = halfColX + col.r * threshold / COLORS;
  //float yOffset = halfColY + col.g * threshold;
  //float cell = floor(col.b * maxColor);
  //
  //float2 lutPos = float2(cell / COLORS + xOffset, yOffset);
  //float4 gradedCol = LookUpTable.Sample(lutSS, lutPos);
  //             
  //return lerp(col, gradedCol, _Contribution);
    
    //Saturate
    color.rgb = saturate(color.rgb);

    //LUT Lookup ��ǥ Ȯ��.
    float lutWidth = lutWidthHeight.x;
    float lutHeight = lutWidthHeight.y;

    float3 lutSize = float3(lutWidth, lutHeight, lutWidth); // Square 2D Layout���� �׻� ���� ������ ����.
    float3 lutIndex = color.rgb * (lutSize - 1.0);

    // 2D LUT ��ǥ ���.
    float2 lutCoord;
    lutCoord.x = (lutIndex.r + lutIndex.g * lutWidth) / (lutWidth * lutWidth);
    lutCoord.y = lutIndex.b / lutHeight;

    // �� �κ� ���ø� �̽� ���ϱ� ���� ������ �߰�.
    lutCoord.x += 0.5 / (lutWidth * lutWidth);
    lutCoord.y += 0.5 / lutHeight;

    //LUT �ؽ��� ���ø�.
    float4 gradedColor = LookUpTable.Sample(lutSS, lutCoord);

    //���� �����ؼ� ���.
    return lerp(color, gradedColor, _Contribution);
}

//�ݵ�� ��ǲ = VOutQuad, �ƿ�ǲ = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    //���� �� ���ø�.
    float4 color = float4(QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz, 1.0f);

    if (gCBuf_IsLutOn)
    {
        color = float4(GetLUTColor(pin.UV, color), 1.0f);
    }
       
    res.Output = color;
    return res;
}