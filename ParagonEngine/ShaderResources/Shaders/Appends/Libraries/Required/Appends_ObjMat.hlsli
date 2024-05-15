#ifndef __DEFINED_APPENDS_OBJMAT_HLSL__
#define __DEFINED_APPENDS_OBJMAT_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"
#include "../Required/Appends_BufferSRVRegisters.hlsli"

//모든 Custom Pixel Shader들이 다른 코드를 적기 전에 사용해야 하는 함수이다.
//Vertex Shader로 하는 것이 더 효율적이지만, 파이프라인을 바꾸지 않고 실행을 위해.
//MaterialID가 기록된 버퍼와 같지 않으면 렌더하지 않는다.

//매 Material마다 업데이트해준다. 같은 Material인지 ID 검사를 하기 위해.
//X : Object ID, Y : Material ID

//이제 objMat이 t12로 합쳐져서 PBR Buffer와 같은 곳으로 전달되면서,
//어쩔 수 없이 float4 중에 xy만을 써야 한다.

cbuffer cbInputMaterial : register(b3)
{
    uint inputID;
};

uint GetObjectID(float2 quadUV)
{
    return asuint(internal_t2_ObjMatAoR.Sample(fullScreenQuadSS, quadUV).x);
}

uint GetMaterialID(float2 quadUV)
{
    return asuint(internal_t2_ObjMatAoR.Sample(fullScreenQuadSS, quadUV).y);
}

//원본
void ClipUnfits(float2 quadUV)
{
    float sampledMatID = asfloat(GetMaterialID(quadUV));

    clip(inputID - sampledMatID + 0.1f);
    clip(sampledMatID - inputID + 0.1f);
}

//Outline Picking을 위해.
void ClipUnfitsObjectID(float2 quadUV)
{
    // 몰랐지만, asfloat, asuint는 일반 캐스팅과 달리 
    // 비트패턴 해석만 다르게 하는 것이다. 
    // 이미 float으로 올려놨기 때문에 놔둔다.
    float sampledMatID = asfloat(GetObjectID(quadUV));

    clip(inputID - sampledMatID + 0.1f);
    clip(sampledMatID - inputID + 0.1f);
}

bool CheckIfFitObjectID(float2 quadUV)
{
    float sampledMatID = asfloat(GetObjectID(quadUV));
    float tVal1 = inputID - sampledMatID + 0.1f;
    float tVal2 = sampledMatID - inputID + 0.1f;
    bool tRet = ((tVal1 >= 0.0f) && (tVal2 >= 0.0f));
    return tRet;
}

#endif //__DEFINED_APPENDS_OBJMAT_HLSL__
