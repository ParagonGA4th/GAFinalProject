///그래픽엔진 자체의 1st Pixel Shader (All)
#pragma pixel_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"

POut1st main(VOut1st input)
{
    POut1st output;
	
    //<Float4>
    //RT0 : Texture UV Coords. (xyz)
    output.pout1st_RT0.xyz = input.vout1st_Tex;
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
    //</Float4>
    
    //<Typeless>
    //RT4 :  Object ID. 일단은 uint로 해석될 예정. (x)
    output.pout1st_RT4.x = input.vout1st_ObjectID;
    //RT4 :  Material ID. 일단은 uint로 해석될 예정. (y)
    output.pout1st_RT4.y = input.vout1st_MaterialID;
    //RT4 : LightMap Texture UV Coords (zw)
    output.pout1st_RT4.zw = input.vout1st_LightmapUV;
    //</Typeless>
    
    return output;
}

