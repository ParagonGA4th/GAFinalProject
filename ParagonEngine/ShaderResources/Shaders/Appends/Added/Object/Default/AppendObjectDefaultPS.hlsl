//기본적으로 Material Path가 없을 시, 로드되는 디폴트 매터리얼 내부사용 셰이더 (VS).
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
    
    //기존 Texture2DArray 기반 Default Material이던 시절.
    //float3 tT2UV3 = GetTex2DArrayUV_F3(pin.UV);
    //res.Output = t2_DiffuseTextureArray.Sample(defaultTextureSS, tT2UV3);
    
    //미리 넣어놓았던 Albedo Map Sampling.
    res.Output = GetAlbedoMap(pin.UV);
    
    return res;
}