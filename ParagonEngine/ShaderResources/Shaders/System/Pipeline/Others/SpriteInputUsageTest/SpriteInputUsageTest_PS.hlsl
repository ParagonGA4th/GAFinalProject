// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929

//PixelShader는 이렇게 따라하면 된다. 출처 : DXTK.

Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

float4 main(float4 color : COLOR0,
    float2 texCoord : TEXCOORD0) : SV_Target0
{
    return Texture.Sample(TextureSampler, texCoord) * color;
}