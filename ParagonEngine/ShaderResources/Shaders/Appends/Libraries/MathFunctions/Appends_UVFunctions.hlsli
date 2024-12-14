#ifndef __DEFINED_APPENDS_UV_FUNCTION_HLSL__
#define __DEFINED_APPENDS_UV_FUNCTION_HLSL__

//UV Tiling.
float2 TileUV(float2 uv, float2 tiling, float2 offset)
{
    return frac(uv * tiling + offset);
}

#endif //__DEFINED_APPENDS_UV_FUNCTION_HLSL__