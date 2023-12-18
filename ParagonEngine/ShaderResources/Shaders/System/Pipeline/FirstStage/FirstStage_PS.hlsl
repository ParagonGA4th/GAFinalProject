///그래픽엔진 자체의 1st Pixel Shader (All)
#pragma pixel_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"

POut1st main(VOut1st input)
{
    POut1st output;
	
    //RT0 : Texture UV Coords.
    output.pout1st_RT0.xyz = input.vout1st_Tex;
    //RT0 : World Space Tangent.x
    output.pout1st_RT0.w = input.vout1st_TangentW.x;
        
    //RT1 : World Space Normal.
    output.pout1st_RT1.xyz = input.vout1st_NormalW;
    //RT1 : World Space Tangent.y
    output.pout1st_RT1.w = input.vout1st_TangentW.y;
        
    //RT2 : World Space Position.
    output.pout1st_RT2.xyz = input.vout1st_PosW;
    //RT2 : World Space Tangent.z
    output.pout1st_RT2.w = input.vout1st_TangentW.z;
        
    //RT3 : 3D Model Color. (나중에 블렌딩에 쓰일 예정)
    output.pout1st_RT3 = input.vout1st_Color;
        
    //RT4 : LightMap Texture UV Coords
    output.pout1st_RT4.xy = input.vout1st_LightmapUV;
    
    //RT4 :  Object ID. 일단은 uint로 해석될 예정. 
    output.pout1st_RT4.z = input.vout1st_ObjectID;
    
    //RT4 :  Alpha (일단 1로 하드코딩될 예정) -> 대다수.
    output.pout1st_RT4.w = input.vout1st_Alpha;
    
    return output;
}

