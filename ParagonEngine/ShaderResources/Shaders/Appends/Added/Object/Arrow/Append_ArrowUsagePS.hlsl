//Appends PS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"

//상수버퍼는 무조건 b8에서 시작. 

//Texture2D는 무조건 t25에서 시작.
Texture2D<float4> t2_DiffuseTexture : register(t25);

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    //모든 Appends의 Pixel Shader 실행 전에, ClipUnfits 함수가 들어있어야 한다. 구조 특성 때문.
    ClipUnfits(pin.UV);
    
    //본격적인 Shader Code.
    POutQuad res;
    
    res.Output = t2_DiffuseTexture.Sample(defaultTextureSS, GetUV_F2(pin.UV));
        
    return res;
}