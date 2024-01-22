//기본적으로 Material Path가 없을 시, 로드되는 디폴트 매터리얼 내부사용 셰이더 (VS).
//Appends PS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"

//Texture2D는 무조건 t25에서 시작.
Texture2DArray<float4> t2_DiffuseTextureArray : register(t25);
Texture2DArray<float4> t2_NormalTextureArray : register(t26);

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    //모든 Appends의 Pixel Shader 실행 전에, ClipUnfits 함수가 들어있어야 한다. 구조 특성 때문.
    ClipUnfits(pin.UV);
    
    //본격적인 Shader Code.
    POutQuad res;
    
    res.Output = t2_DiffuseTextureArray.Sample(defaultTextureSS, GetTex2DArrayUV_F3(pin.UV));
    // Normal은 일단은 반영하지 않았음.
        
    return res;
}