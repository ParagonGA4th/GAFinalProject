///그래픽엔진 자체의 1st Pixel Shader (All)
#pragma pixel_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../../Appends/Libraries/TextureBuffers/Appends_InstancedValues.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"

Texture2DArray<float4> AlbedoTextureArray : register(t8);
//Texture2DArray<float4> NormalTextureArray : register(t9);
//Texture2DArray<float4> ArmTextureArray : register(t10);
Texture2DArray<float4> AlphaTextureArray : register(t11);

const static float ALPHA_CLIPPING_THRESHOLD = 0.5f;

POut1st_Total main(VOut1st_Instanced input)
{
    POut1st_Total output;
    float3 tT2UV3 = float3(input.vout1st_Tex, input.vout1st_MeshMatID);
    clip(AlphaTextureArray.Sample(defaultTextureSS, tT2UV3) - ALPHA_CLIPPING_THRESHOLD);
    
    //<Float4>
    //RT0 : Texture UV Coords. (xy)
    output.pout1st_RT0.xy = input.vout1st_Tex;
    //RT0 : Mesh Material ID. (z)
    output.pout1st_RT0.z = input.vout1st_MeshMatID;
    //RT0 : W Divide Depth
    //output.pout1st_RT0.w = input.vout1st_PosH.w / input.vout1st_PosH.z;
    output.pout1st_RT0.w = input.vout1st_PosH.z / input.vout1st_PosH.w;
    
    //RT1 : World Space Normal. (xyz)
    output.pout1st_RT1.xyz = input.vout1st_NormalW;
    //RT1 : vout1st_LightPixelPos.x (w)
    output.pout1st_RT1.w = input.vout1st_LightPixelPos.x;
        
    //RT2 : World Space Position. (xyz)
    output.pout1st_RT2.xyz = input.vout1st_PosW;
    //RT2 : vout1st_LightPixelPos.y (w)
    output.pout1st_RT2.w = input.vout1st_LightPixelPos.y;
        
    //RT3 : World Space Tangent (xyz)
    output.pout1st_RT3.xyz = input.vout1st_TangentW;
    //RT3 : vout1st_LightPixelPos.z (w)
    output.pout1st_RT3.w = input.vout1st_LightPixelPos.z;

    //RT4 : LightMap Sample Value (xyz) + Lightmapping이 활용되었는지(w). 음수 : NO, 양수 : YES. -> 그러니 여기에서는 양수. // 2: Alpha Clip.
    output.pout1st_RT4.xyz = GetLightmapData(input.vout1st_LightmapUV, input.vout1st_InstanceID).xyz;
    output.pout1st_RT4.w = 2.0f;
    //</Float4>
    
    //5,6,7
    //float3 tARMSampleVal = ArmTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    //인스턴싱이라서 사용하지 않음!
    float3 tZeroVal = float3(1.f,1.f,1.f);
    //float3 tZeroVal = tARMSampleVal;
    
    //ObjMat 전달.
    output.pout_ObjMatAoR.x = input.vout1st_ObjID;
    output.pout_ObjMatAoR.y = input.vout1st_MatID;
     //Ambient Occlusion 값 전달.
    output.pout_ObjMatAoR.z = tZeroVal.x;
    //Roughness Map 값 전달.
    output.pout_ObjMatAoR.w = tZeroVal.y;
    //Albedo Map 값 전달.
    output.pout_AlbedoMetallic.xyz = AlbedoTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    //Metallic Map 전달.
    output.pout_AlbedoMetallic.w = tZeroVal.z;
    //Normal Map 전달.
    //output.pout_NormalAlpha.xyz = NormalTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    output.pout_NormalAlpha.xyz = tZeroVal;
    //Alpha를 전달하지는 않는다.
    
    return output;
}

