///그래픽엔진 자체의 1st Pixel Shader (All)
#pragma pixel_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../../Appends/Libraries/TextureBuffers/Appends_InstancedValues.hlsli"

POut1st main(VOut1st_Instanced input)
{
    POut1st output;
	
    //<Float4>
    //RT0 : Texture UV Coords. (xy)
    output.pout1st_RT0.xy = input.vout1st_Tex;
    //RT0 : Mesh Material ID. (z)
    output.pout1st_RT0.z = input.vout1st_MeshMatID;
    //RT0 : W Divide Depth
    output.pout1st_RT0.w = input.vout1st_PosH.w / input.vout1st_PosH.z;
    
    //RT1 : World Space Normal. (xyz)
    output.pout1st_RT1.xyz = input.vout1st_NormalW;
    //RT1 : Vertex Color.x (w)
    output.pout1st_RT1.w = input.vout1st_Color.x;
        
    //RT2 : World Space Position. (xyz)
    output.pout1st_RT2.xyz = input.vout1st_PosW;
    //RT2 :  Vertex Color.y (w)
    output.pout1st_RT2.w = input.vout1st_Color.y;
        
    //RT3 : World Space Tangent (xyz)
    output.pout1st_RT3.xyz = input.vout1st_TangentW;
    //RT3 :  Vertex Color.z (w)
    output.pout1st_RT3.w = input.vout1st_Color.z;

    //RT4 : LightMap Sample Value (xyz) + Lightmapping이 활용되었는지(w). 음수 : NO, 양수 : YES. -> 그러니 여기에서는 양수.
    output.pout1st_RT4.xyz = GetLightmapData(input.vout1st_LightmapUV, input.vout1st_InstanceID).xyz;
    output.pout1st_RT4.w = 1.0f;
    //</Float4>
    
    //5,6,7
    return output;
}

