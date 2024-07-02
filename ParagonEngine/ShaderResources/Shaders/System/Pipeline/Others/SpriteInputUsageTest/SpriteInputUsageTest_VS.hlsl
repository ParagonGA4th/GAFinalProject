// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929

//Vertex Shader는 이렇게 따라하면 된다. DXTK 출처.
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
    //SpriteBatch 구현체 기준으로 MatrixTransform이었음.
    position = mul(position, PredefinedIdentity);
}

//실제 셰이더는 똑같이 Identity가 들어가야 한다.