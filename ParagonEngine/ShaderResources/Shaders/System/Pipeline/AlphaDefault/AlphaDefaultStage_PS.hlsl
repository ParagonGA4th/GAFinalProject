///그래픽엔진 자체의 1st Pixel Shader (All)
#pragma pixel_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../Libraries/System_AlphaDefaultInfo.hlsli"

Texture2DArray<float4> AlbedoTextureArray : register(t8);
Texture2DArray<float4> NormalTextureArray : register(t9);
Texture2DArray<float4> ArmTextureArray : register(t10);
//Alpha는 바인딩하지 않는다.

POutQuad main(VOut1st input)
{
    POutQuad output;
	
    float3 tT2UV3 = float3(input.vout1st_Tex, input.vout1st_MeshMatID);
     // float3 tARMSampleVal = ArmTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    
    //테스트 때는 그냥 BaseColor만 내보내보자.
    float3 tAlbedo = AlbedoTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    
    //알파 반영해서 내보내기.
    output.Output = float4(tAlbedo, gCBuf_ObjectAlpha);
    
    return output;
}

