
#ifndef __DEFINED_SYSTEM_DYNAMIC_INTERFACES_VIEWPROJ_HLSL__
#define __DEFINED_SYSTEM_DYNAMIC_INTERFACES_VIEWPROJ_HLSL__

#include "../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

interface IViewProjGettable
{
    float4x4 GetAssignedViewProj();
};

class CCameraViewProjGet : IViewProjGettable
{
    //무슨 러시아 포럼에서 봤는데 이래야 인식이 된다고 한다. 변수를 사용해야 메모리 할당? 등인듯.
    //float tCCameraViewProjGet_Pad;
    float4x4 GetAssignedViewProj()
    {
        return gCBuf_ViewProjMatrix;
    }
};

class CMainLightViewProjGet : IViewProjGettable
{
    //float tCMainLightViewProjGet_Pad;
    float4x4 GetAssignedViewProj()
    {
        return _lightViewProj;
    }
};

IViewProjGettable g_ViewProjGetter;

//이거 받아서, 자식 클래스에서 구현해야.

#endif //__DEFINED_SYSTEM_DYNAMIC_INTERFACES_HLSL__