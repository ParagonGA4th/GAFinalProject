//�ʿ��� ��Ҹ� ��� ���������� ������ ����.

#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../Appends/Libraries/Required/Appends_ObjMat.hlsli"

//�ݵ�� ��ǲ = VOutQuad, �ƿ�ǲ = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
   
    //�ʿ��� ��Ҹ� ����ϱ� ���ؼ�. 
    //�ٽ� �ѹ� ObjMat���� ���̶���Ʈ�� ������Ʈ�� �ش�Ǵ� �κи� �޴´�.
   ClipUnfitsObjectID(pin.UV);
   
   //float4 tOutlineColor = { 0.871f, 0.996f, 0.f, 1.f };
   float4 tOutlineColor = { 0.0f, 1.f, 0.f, 1.f };
   
   //Stencil�� ���������� ������ ����ϴ� ���ÿ�, ������ ĥ�Ѵ�.
   res.Output = tOutlineColor;
    
    return res;
}