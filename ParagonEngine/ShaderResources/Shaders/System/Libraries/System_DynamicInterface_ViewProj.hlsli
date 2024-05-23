
#ifndef __DEFINED_SYSTEM_DYNAMIC_INTERFACES_VIEWPROJ_HLSL__
#define __DEFINED_SYSTEM_DYNAMIC_INTERFACES_VIEWPROJ_HLSL__

#include "../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

interface IViewProjGettable
{
    float4x4 GetAssignedViewProj();
};

class CCameraViewProjGet : IViewProjGettable
{
    float4x4 GetAssignedViewProj()
    {
        return gCBuf_ViewProjMatrix;
    }
};

class CMainLightViewProjGet : IViewProjGettable
{
    float4x4 GetAssignedViewProj()
    {
        return _lightViewProj;
    }
};

IViewProjGettable g_ViewProjGetter;

//이거 받아서, 자식 클래스에서 구현해야.

#endif //__DEFINED_SYSTEM_DYNAMIC_INTERFACES_HLSL__