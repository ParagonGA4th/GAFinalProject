#ifndef __DEFINED_APPENDS_BUFFER_REGISTER_SRV_HLSL__
#define __DEFINED_APPENDS_BUFFER_REGISTER_SRV_HLSL__

//¸ðµÎ RGB / A. Quad-Based.
Texture2D<float4> internal_t2_ObjMatAoR : register(t12);
Texture2D<float4> internal_t2_AlbedoMetallic : register(t13);
Texture2D<float4> internal_t2_NormalAlpha : register(t14);

#endif //__DEFINED_APPENDS_BUFFER_REGISTER_SRV_HLSL__
