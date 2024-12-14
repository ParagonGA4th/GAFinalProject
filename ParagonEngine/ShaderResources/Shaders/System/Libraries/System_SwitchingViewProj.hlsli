
#ifndef __DEFINED_SYSTEM_SWITCHING_VIEWPROJ_HLSL__
#define __DEFINED_SYSTEM_SWITCHING_VIEWPROJ_HLSL__

#include "../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

//interface IViewProjGettable
//{
//    float4x4 GetAssignedViewProj();
//};
//
//class CCameraViewProjGet : IViewProjGettable
//{
//    //���� ���þ� �������� �ôµ� �̷��� �ν��� �ȴٰ� �Ѵ�. ������ ����ؾ� �޸� �Ҵ�? ���ε�.
//    //float tCCameraViewProjGet_Pad;
//    float4x4 GetAssignedViewProj()
//    {
//        return gCBuf_ViewProjMatrix;
//    }
//};
//
//class CMainLightViewProjGet : IViewProjGettable
//{
//    //float tCMainLightViewProjGet_Pad;
//    float4x4 GetAssignedViewProj()
//    {
//        return _lightViewProj;
//    }
//};
//
//IViewProjGettable g_ViewProjGetter;

cbuffer cbSwitchingViewProj : register(b1)
{
    float4x4 gCBuf_SwitchableViewProj;
};

//�̰� �޾Ƽ�, �ڽ� Ŭ�������� �����ؾ�.

#endif //__DEFINED_SYSTEM_SWITCHING_VIEWPROJ_HLSL__