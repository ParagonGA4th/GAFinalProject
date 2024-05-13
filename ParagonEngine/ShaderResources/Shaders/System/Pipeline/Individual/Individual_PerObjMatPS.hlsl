//#include "../../Libraries/System_PerObjMatLayouts.hlsli"
//#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"

//Texture2DArray<float4> AlbedoTextureArray    : register(t8);
//Texture2DArray<float4> NormalTextureArray    : register(t9);
//Texture2DArray<float4> ArmTextureArray       : register(t10);
//Texture2DArray<float4> AlphaTextureArray     : register(t11);
//// Specular 제거, 대신 Alpha 추가.

//POutPerObjMat main(VOutPerObjMat input)
//{
//    POutPerObjMat output;
    
//    //<uint2>
//    //Object ID 전달.
//    output.pout_ObjMatAoR.x = input.vout1st_ObjID;
//    //Material ID 전달.
//    output.pout_ObjMatAoR.y = input.vout1st_MatID;
    
//    float3 tT2UV3 = float3(input.vout1st_Tex, input.vout1st_MeshMatID);
//    float3 tARMSampleVal = ArmTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    
//    //Ambient Occlusion 값 전달.
//    output.pout_ObjMatAoR.z = tARMSampleVal.x;
//    //Roughness Map 값 전달.
//    output.pout_ObjMatAoR.w = tARMSampleVal.y;
//    //Albedo Map 값 전달.
//    output.pout_AlbedoMetallic.xyz = AlbedoTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
//    //Metallic Map 전달.
//    output.pout_AlbedoMetallic.w = tARMSampleVal.z;
//    //Normal Map 전달.
//    output.pout_NormalAlpha.xyz = NormalTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
//    //Alpha Map 전달.
//    output.pout_NormalAlpha.w = AlphaTextureArray.Sample(defaultTextureSS, tT2UV3).x;
    
//    return output;
//}