///그래픽엔진 자체의 1st Pixel Shader (All)
#pragma pixel_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"

POut1st main(VOut1st input)
{
    POut1st output;
	
    //<Float4>
    //RT0 : Texture UV Coords. (xy)
    output.pout1st_RT0.xy = input.vout1st_Tex;
    //RT0 : Mesh Material ID. (z)
    output.pout1st_RT0.z = input.vout1st_MeshMatID;
    //RT0 : Alpha (w)
    output.pout1st_RT0.w = input.vout1st_Alpha;
    
    //RT1 : World Space Normal. (xyz)
    output.pout1st_RT1.xyz = input.vout1st_NormalW;
    //RT1 : World Space Tangent.x (w)
    output.pout1st_RT1.w = input.vout1st_TangentW.x;
        
    //RT2 : World Space Position. (xyz)
    output.pout1st_RT2.xyz = input.vout1st_PosW;
    //RT2 : World Space Tangent.y (w)
    output.pout1st_RT2.w = input.vout1st_TangentW.y;
        
    //RT3 : 3D Model Color. (나중에 블렌딩에 쓰일 예정) (xyz)
    output.pout1st_RT3.xyz = input.vout1st_Color.xyz;
    //RT3 : World Space Tangent.z (w)
    output.pout1st_RT3.w = input.vout1st_TangentW.z;

    //RT4 : UV Set2 (float2) (x)
    output.pout1st_RT4.xy = input.vout1st_UVSet2;
    //RT4 : LightMap Texture UV Coords (zw)
    output.pout1st_RT4.zw = input.vout1st_LightmapUV;
    //</Float4>
    
    return output;
}

