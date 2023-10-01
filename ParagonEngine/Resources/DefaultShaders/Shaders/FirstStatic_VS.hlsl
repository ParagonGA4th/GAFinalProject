///그래픽엔진 자체의 1st Vertex Shader (Static)

#include "../Libraries/DefaultBufferPerObject.hlsl"
#include "../Libraries/DefaultLayouts.hlsl"

VOut1st VS_MAIN(Vin1stStatic input)
{
    VOut1st output;
    
    // Position을 Local -> World 이동.
    output.vout1st_PosW = mul(float4(input.vin1st_PosL, 1.0f), gCBuf_World).xyz;
    // Normal을 Local -> World 이동.
    output.vout1st_NormalW = mul(input.vin1st_NormalL, (float3x3)gCBuf_WorldInvTranspose);
    // Tangent를 Local -> World 이동.
    output.vout1st_TangentW = mul(input.vin1st_TangentL, (float3x3)gCBuf_World);
	// 동차좌표계 내 Position 계산.
    output.vout1st_PosH = mul(float4(input.vin1st_PosL, 1.0f), gCBuf_WorldViewProj);
	// Color & UV(W) 값 전달.
    output.vout1st_Color = input.vin1st_Color;
    output.vout1st_Tex = input.vin1st_Tex;
	//추후에 Material ID가 로직에 들어가면 이 역시 추가해야 한다.
    
    return output;
}

