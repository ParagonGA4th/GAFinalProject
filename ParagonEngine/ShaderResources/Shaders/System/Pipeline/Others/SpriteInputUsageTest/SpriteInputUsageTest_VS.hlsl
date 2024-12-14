// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929

//Vertex Shader�� �̷��� �����ϸ� �ȴ�. DXTK ��ó.
//cbuffer Parameters : register(b0)
//{
//    row_major float4x4 MatrixTransform;
//};

const static float4x4 PredefinedIdentity = 
{
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1
};

void main(inout float4 color : COLOR0,
    inout float2 texCoord : TEXCOORD0,
    inout float4 position : SV_Position)
{
    //SpriteBatch ����ü �������� MatrixTransform�̾���.
    position = mul(position, PredefinedIdentity);
}

//���� ���̴��� �Ȱ��� Identity�� ���� �Ѵ�.