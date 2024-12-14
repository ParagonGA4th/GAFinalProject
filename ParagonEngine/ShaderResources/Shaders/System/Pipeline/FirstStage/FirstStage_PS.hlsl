///�׷��ȿ��� ��ü�� 1st Pixel Shader (All)
#pragma pixel_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"

Texture2DArray<float4> AlbedoTextureArray : register(t8);
Texture2DArray<float4> NormalTextureArray : register(t9);
Texture2DArray<float4> ArmTextureArray : register(t10);
//Alpha�� ���ε����� �ʴ´�.

POut1st_Total main(VOut1st input)
{
    //�ν��Ͻ� �����̶� ������ �� �� ����. 
    //SV_InstanceID ������.
    POut1st_Total output;
	
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
    //RT1 : LightPixelPos.x (w)
    //output.pout1st_RT1.w = input.vout1st_Color.x;
    output.pout1st_RT1.w = input.vout1st_LightPixelPos.x;
        
    //RT2 : World Space Position. (xyz)
    output.pout1st_RT2.xyz = input.vout1st_PosW;
    //RT2 :  LightPixelPos.y (w)
    output.pout1st_RT2.w = input.vout1st_LightPixelPos.y;
        
    //RT3 : World Space Tangent (xyz)
    output.pout1st_RT3.xyz = input.vout1st_TangentW;
    //RT3 :  LightPixelPos.z (w)
    output.pout1st_RT3.w = input.vout1st_LightPixelPos.z;

    //�� �ν��Ͻ̵� ���� ���, ������ 0�� ����Ѵ�. (SampledValue)
    //RT4 : LightMap Sample Value (xyz) + Lightmapping�� Ȱ��Ǿ�����(w). ���� : NO, ��� : YES.
    output.pout1st_RT4.xyz = float3(0.0f, 0.0f, 0.0f);
    output.pout1st_RT4.w = -1.0f; // ����Ʈ������ Ȱ����� �ʾҴ�!
    
    float3 tT2UV3 = float3(input.vout1st_Tex, input.vout1st_MeshMatID);
    float3 tARMSampleVal = ArmTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    
    //ObjMat ����.
    output.pout_ObjMatAoR.x = gCBuf_ObjID;
    output.pout_ObjMatAoR.y = gCBuf_MatID;
     //Ambient Occlusion �� ����.
    output.pout_ObjMatAoR.z = tARMSampleVal.x;
    //Roughness Map �� ����.
    output.pout_ObjMatAoR.w = tARMSampleVal.y;
    //Albedo Map �� ����.
    output.pout_AlbedoMetallic.xyz = AlbedoTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    //Metallic Map ����.
    output.pout_AlbedoMetallic.w = tARMSampleVal.z;
    //Normal Map ����.
    output.pout_NormalAlpha.xyz = NormalTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;

    //Alpha Map�� ���޵��� �ʴ´� ���⼭! ����� Opaque �����̴�.
    
    return output;
}

