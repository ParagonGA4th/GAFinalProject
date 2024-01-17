#ifndef __DEFINED_APPENDS_CLIP_UNFIT_HLSL__
#define __DEFINED_APPENDS_CLIP_UNFIT_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"

//모든 Custom Pixel Shader들이 다른 코드를 적기 전에 사용해야 하는 함수이다.
//Vertex Shader로 하는 것이 더 효율적이지만, 파이프라인을 바꾸지 않고 실행을 위해.
//MaterialID가 기록된 버퍼와 같지 않으면 렌더하지 않는다.

//매 Material마다 업데이트해준다. 같은 Material인지 ID 검사를 하기 위해.
//X : Object ID, Y : Material ID
Texture2D<float2> _objMatSRV : register(t3);

cbuffer cbInputMaterial : register(b3)
{
    uint inputMatID;
}

uint GetObjectID(float2 quadUV)
{
    return asuint(_objMatSRV.Sample(fullScreenQuadSS, quadUV).x);
}

uint GetMaterialID(float2 quadUV)
{
    return asuint(_objMatSRV.Sample(fullScreenQuadSS, quadUV).y);
}

//원본
void ClipUnfits(float2 quadUV)
{
    float sampledMatID = asfloat(GetMaterialID(quadUV));

    clip(inputMatID - sampledMatID + 0.1f);
    clip(sampledMatID - inputMatID + 0.1f);
}


#endif //__DEFINED_APPENDS_CLIP_UNFIT_HLSL__
