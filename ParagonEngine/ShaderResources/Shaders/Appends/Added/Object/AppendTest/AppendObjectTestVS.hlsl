//Appends VS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_VSCommon.hlsli"

//다른 헤더들을 추가할 수 있을 것이다.
cbuffer cbAppendsObjectVS : register(b8)
{
    bool tTest; 
    uint tUintExample;
    int tIntExample;
    float tFloat1Test;
    float2 tFloat2Test;
    float3 tFloat3Test;
    float4 tFloat4Test;
    float4 tColor4Test1;
    float4 tColor4Test2;  
};

//반드시 인풋 = VinQuad, 아웃풋 = VOutQuad.
VOutQuad main(VinQuad vin)
{
    VOutQuad vout;
    
     vout.PositionH = float4(vin.Position, 1.0f);
  //  vout.PositionH = float4(vin.Position, tFloat1Test);
    vout.UV = vin.UV;
	
    return vout;
}

