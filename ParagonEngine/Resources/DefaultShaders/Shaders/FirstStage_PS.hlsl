///그래픽엔진 자체의 1st Pixel Shader (All)

#include "../Libraries/DefaultLayouts.hlsli"
#include "../Libraries/misc.hlsli"


Texture2D Diffuse;

POut1st PS_MAIN(VOut1st input)
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
    
    //RT3 : 3D Model Color.
	output.pout1st_RT3 = input.vout1st_Color;
    
    //RT4 : Material ID. (위에서 일단 하드코딩 되어 0으로 내려옴)
	output.pout1st_RT4.x = input.vout1st_MatID;
    //RT4 : Screen Space Velocity. (0,0으로 하드코딩됨)
	output.pout1st_RT4.yz = float2(0.0f, 0.0f);
    //RT4 : Alpha. (1로 하드코딩됨)
	output.pout1st_RT4.w = 1.0f;
    
    //RT5 : Temp Diffuse Texture
    output.pout1st_RT5 = Diffuse.Sample(state, input.vout1st_Tex.xy);
	
	//RT6 : Phong Lighting Results
  
	//pout1st_Depth -> SV_Depth에 대해서는 아직은 추가적인 처리 안 해줄 것.
    
    
    return output;
}

