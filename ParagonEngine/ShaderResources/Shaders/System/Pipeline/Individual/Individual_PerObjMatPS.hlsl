#include "../../Libraries/System_PerObjMatLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"

Texture2DArray<float4> AlbedoTextureArray    : register(t8);
Texture2DArray<float4> NormalTextureArray    : register(t9);
Texture2DArray<float4> SpecularTextureArray  : register(t10);
Texture2DArray<float4> ArmTextureArray       : register(t11);

POutPerObjMat main(VOutPerObjMat input)
{
    POutPerObjMat output;
    
    //<uint2>
    //Object ID 전달.
    output.pout_ObjMat.x = input.vout1st_ObjID;
    //Material ID 전달.
    output.pout_ObjMat.y = input.vout1st_MatID;
    
    float3 tT2UV3 = float3(input.vout1st_Tex, input.vout1st_MeshMatID);
    
    //ARM 제외 텍스쳐 할당.
    output.pout_AlbedoAO.xyz = AlbedoTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    output.pout_NormalRoughness.xyz = NormalTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    output.pout_SpecularMetallic.xyz = SpecularTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    
    //ARM 텍스쳐 할당.
    float3 tARMSampleVal = ArmTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    output.pout_AlbedoAO.w = tARMSampleVal.x;
    output.pout_NormalRoughness.w = tARMSampleVal.y;
    output.pout_SpecularMetallic.w = tARMSampleVal.z;
    
    return output;
}