//필요한 요소만 골라서 일차적으로 렌더링 수행.

#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../../Appends/Libraries/Appends_PSCommon.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
   
    float4 worldPosition = float4(GetPosition(pin.UV), 1.0f);
    float4 homogeneous = mul(worldPosition, _lightViewProj);
    float depth = homogeneous.z / homogeneous.w;
    res.Output = float4(depth, depth, depth, 1.0f); //Light 기준 Depth 기록.
    
    //일단은, Light 기준 Depth 기록.
    return res;
}