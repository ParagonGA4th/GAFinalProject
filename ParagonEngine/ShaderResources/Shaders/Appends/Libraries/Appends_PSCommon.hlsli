///모든 Engine Append(Render) Shader가 작동하기 위해서는, 기본적으로 필요한 셰이더이다.

#ifndef __DEFINED_APPENDS_PS_COMMON_HLSL__
#define __DEFINED_APPENDS_PS_COMMON_HLSL__

//System
#include "../../System/Libraries/System_QuadLayouts.hlsli"

//Appends
#include "TextureBuffers/Appends_GBufferTextures.hlsli"
#include "TextureBuffers/Appends_PBRBufferTextures.hlsli"
#include "TextureBuffers/Appends_InstancedValues.hlsli"
#include "MathFunctions/Appends_ShadowFunctions.hlsli"

//Required
#include "Required/Appends_ObjMat.hlsli"



#endif //__DEFINED_APPENDS_PS_COMMON_HLSL__
