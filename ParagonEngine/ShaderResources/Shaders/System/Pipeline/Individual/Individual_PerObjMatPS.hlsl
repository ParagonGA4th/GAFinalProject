#include "../../Libraries/System_PerObjMatLayouts.hlsli"
#include "../../../Appends/Libraries/TextureBuffers/Appends_GBufferTextures.hlsli"

Texture2DArray<float4> t2_AlbedoTextureArray  : register(t8);
Texture2DArray<float4> t2_NormalTextureArray   : register(t9);
Texture2DArray<float4> t2_SpecularTextureArray : register(t10);
Texture2DArray<float4> t2_ArmTextureArray      : register(t11);

POutPerObjMat main(VOutPerObjMat input)
{
    POutPerObjMat output;
    
    //<uint2>
    //Object ID 전달.
    output.pout_ObjMat.x = input.vout1st_ObjID;
    //Material ID 전달.
    output.pout_ObjMat.y = input.vout1st_MatID;
    
   // output.pout_AlbedoAO.xyz = 
    return output;
}