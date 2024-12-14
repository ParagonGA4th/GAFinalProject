#ifndef __DEFINED_APPENDS_OBJMAT_HLSL__
#define __DEFINED_APPENDS_OBJMAT_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"
#include "../Required/Appends_BufferSRVRegisters.hlsli"

//��� Custom Pixel Shader���� �ٸ� �ڵ带 ���� ���� ����ؾ� �ϴ� �Լ��̴�.
//Vertex Shader�� �ϴ� ���� �� ȿ����������, ������������ �ٲ��� �ʰ� ������ ����.
//MaterialID�� ��ϵ� ���ۿ� ���� ������ �������� �ʴ´�.

//�� Material���� ������Ʈ���ش�. ���� Material���� ID �˻縦 �ϱ� ����.
//X : Object ID, Y : Material ID

//���� objMat�� t12�� �������� PBR Buffer�� ���� ������ ���޵Ǹ鼭,
//��¿ �� ���� float4 �߿� xy���� ��� �Ѵ�.

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

//����
void ClipUnfits(float2 quadUV)
{
    float sampledMatID = asfloat(GetMaterialID(quadUV));

    clip(inputID - sampledMatID + 0.1f);
    clip(sampledMatID - inputID + 0.1f);
}

//Outline Picking�� ����.
void ClipUnfitsObjectID(float2 quadUV)
{
    // ��������, asfloat, asuint�� �Ϲ� ĳ���ð� �޸� 
    // ��Ʈ���� �ؼ��� �ٸ��� �ϴ� ���̴�. 
    // �̹� float���� �÷����� ������ ���д�.
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
