//필요한 요소만 골라서 일차적으로 렌더링 수행.

#include "../../../Libraries/System_QuadLayouts.hlsli"
#include "../../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"
#include "../../../../Appends/Libraries/TextureBuffers/Appends_GBufferTextures.hlsli"
#include "../../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

Texture2D<float4> QuadRTV : register(t5);

Texture2D<float4> LookUpTable : register(t1);

//LUT에 있는 색깔 개수.
const static float COLORS = 32.0f;
const static float _Contribution = 0.2f; // 0-1 사이. 보간 비율이다.

//유니티 기준으로, NameHack이 있다.
//float4 _LUT_TexelSize -> NNN_TexelSize로 끝나면, z는 NNN의 width, w는 NNN의 height을 반환.
// 결국 텍스쳐의 Size값을 받는 코드.
cbuffer cbLutWidthHeight : register(b0)
{
    //x와 y만 쓰인다.
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

    //LUT Lookup 좌표 확인.
    float lutWidth = lutWidthHeight.x;
    float lutHeight = lutWidthHeight.y;

    float3 lutSize = float3(lutWidth, lutHeight, lutWidth); // Square 2D Layout으로 항상 들어올 것으로 가정.
    float3 lutIndex = color.rgb * (lutSize - 1.0);

    // 2D LUT 좌표 계산.
    float2 lutCoord;
    lutCoord.x = (lutIndex.r + lutIndex.g * lutWidth) / (lutWidth * lutWidth);
    lutCoord.y = lutIndex.b / lutHeight;

    // 끝 부분 샘플링 이슈 피하기 위해 오프셋 추가.
    lutCoord.x += 0.5 / (lutWidth * lutWidth);
    lutCoord.y += 0.5 / lutHeight;

    //LUT 텍스쳐 샘플링.
    float4 gradedColor = LookUpTable.Sample(lutSS, lutCoord);

    //값을 보간해서 출력.
    return lerp(color, gradedColor, _Contribution);
}

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    //원래 값 샘플링.
    float4 color = float4(QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz, 1.0f);

    if (gCBuf_IsLutOn)
    {
        color = float4(GetLUTColor(pin.UV, color), 1.0f);
    }
       
    res.Output = color;
    return res;
}