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
    float tVal = GetDepth(tActualUV);
    
    //0이랑 가까울수록 카메라 근접,
    //1 가까울수록 카메라 멀리.
    float fogRatio = 0.5f;
    if (tVal > fogRatio)
    {
        //영향을 받는 범위.
        //실제로 White로 표시.
        res.Output = float4(tVal, tVal, tVal, 1.0f);
        
        
    }
    else
    {
        //영향을 안 받는 범위.
        res.Output = float4(0.0f, 1.0f, 0.0f, 1.0f);
    }

    return res;
}