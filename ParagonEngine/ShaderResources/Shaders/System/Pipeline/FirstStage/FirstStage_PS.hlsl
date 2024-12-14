///그래픽엔진 자체의 1st Pixel Shader (All)
#pragma pixel_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"

Texture2DArray<float4> AlbedoTextureArray : register(t8);
Texture2DArray<float4> NormalTextureArray : register(t9);
Texture2DArray<float4> ArmTextureArray : register(t10);
//Alpha는 바인딩하지 않는다.

POut1st_Total main(VOut1st input)
{
    //인스턴스 버전이랑 통일은 할 수 없다. 
    //SV_InstanceID 때문에.
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

    //비 인스턴싱된 값의 경우, 무조건 0을 기록한다. (SampledValue)
    //RT4 : LightMap Sample Value (xyz) + Lightmapping이 활용되었는지(w). 음수 : NO, 양수 : YES.
    output.pout1st_RT4.xyz = float3(0.0f, 0.0f, 0.0f);
    output.pout1st_RT4.w = -1.0f; // 라이트매핑이 활용되지 않았다!
    
    float3 tT2UV3 = float3(input.vout1st_Tex, input.vout1st_MeshMatID);
    float3 tARMSampleVal = ArmTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    
    //ObjMat 전달.
    output.pout_ObjMatAoR.x = gCBuf_ObjID;
    output.pout_ObjMatAoR.y = gCBuf_MatID;
     //Ambient Occlusion 값 전달.
    output.pout_ObjMatAoR.z = tARMSampleVal.x;
    //Roughness Map 값 전달.
    output.pout_ObjMatAoR.w = tARMSampleVal.y;
    //Albedo Map 값 전달.
    output.pout_AlbedoMetallic.xyz = AlbedoTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;
    //Metallic Map 전달.
    output.pout_AlbedoMetallic.w = tARMSampleVal.z;
    //Normal Map 전달.
    output.pout_NormalAlpha.xyz = NormalTextureArray.Sample(defaultTextureSS, tT2UV3).xyz;

    //Alpha Map은 전달되지 않는다 여기서! 여기는 Opaque 전용이니.
    
    return output;
}

