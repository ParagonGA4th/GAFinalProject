//필요한 요소만 골라서 일차적으로 렌더링 수행.

#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../Appends/Libraries/Required/Appends_ObjMat.hlsli"

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
   
    //필요한 요소만 출력하기 위해서. 
    //다시 한번 ObjMat에서 하이라이트된 오브젝트와 해당되는 부분만 받는다.
   ClipUnfitsObjectID(pin.UV);
   
   //float4 tOutlineColor = { 0.871f, 0.996f, 0.f, 1.f };
   float4 tOutlineColor = { 0.0f, 1.f, 0.f, 1.f };
   
   //Stencil을 파이프라인 상으로 기록하는 동시에, 색깔을 칠한다.
   res.Output = tOutlineColor;
    
    return res;
}