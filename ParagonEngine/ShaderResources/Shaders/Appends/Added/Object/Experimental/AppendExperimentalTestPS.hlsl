//Appends PS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    //모든 Appends의 Pixel Shader 실행 전에, ClipUnfits 함수가 들어있어야 한다. 구조 특성 때문.
    ClipUnfits(pin.UV);
    
    //본격적인 Shader Code.
    POutQuad res;
    
    float2 tActualUV = GetUV_F2(pin.UV);
    float depthValue = GetPseudoLinearDepth(tActualUV);
    
    res.Output = float4(depthValue, depthValue, depthValue, 1.0f);
    
   //// 처음 10% 범위는 빨간색
   //if (depthValue > 0.5f)
   //{
   //    res.Output = float4(1.0, 0.0f, 0.0f, 1.0f);
   //}
   //
   //// 다음 0.025% 범위는 녹색
   //if (depthValue > 0.6f)
   //{
   //    res.Output = float4(0.0, 1.0f, 0.0f, 1.0f);
   //}
   //
   //// 나머지 깊이 버퍼는 파란색
   //if (depthValue > 0.7f)
   //{
   //    res.Output = float4(0.0, 0.0f, 1.0f, 1.0f);
   //}
   //
   ////더 깊으면?
   //if (depthValue > 0.8f)
   //{
   //    res.Output = float4(1.0f, 0.0f, 1.0f, 1.0f);
   //}
   //
   //if (depthValue > 0.9f)
   //{
   //    res.Output = float4(1.0f, 1.0f, 0.0f, 1.0f);
   //}
   //
   //if (depthValue > 0.95f)
   //{
   //    res.Output = float4(0.0f, 1.0f, 1.0f, 1.0f);
   //}
    
    return res;
}