//�ʿ��� ��Ҹ� ��� ���������� ������ ����.

#include "../../../Libraries/System_QuadLayouts.hlsli"
#include "../../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"
#include "../../../../Appends/Libraries/TextureBuffers/Appends_GBufferTextures.hlsli"

Texture2D<float4> QuadRTV : register(t5);

static const float FOG_START_LEVEL_Y = -100.0f;
static const float FOG_END_LEVEL_Y = -150.0f;
static const float FOG_DENSITY = 0.05f;
//�ݵ�� ��ǲ = VOutQuad, �ƿ�ǲ = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    
    //��ü�� ToGBuffer���� ������ �ǵ��ۿ� ����. UV ��ȯ�� �ι� ��ĥ �ʿ䰡 ����.
    //�̰� ���� ���������� ���� => �ٸ� �뵵�� ����ؾ�!
    float4 originalColor = float4(QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz, 1.0f);
    
    //���� Height�� ���� ���غ��� ������ : Cubemap ���� �̹� ������ / �̸� ��� �ѱ� �� ���� ���̴�.
    float3 worldPos = GetPosition(pin.UV);
    float height = worldPos.y;
   
    float4 staticFogColor = float4(0.6627f, 0.745f, 0.8313f, 1.0f);
    
    float viewVal = gCBuf_EyePosition.y - worldPos.y;
    
    //float viewDistance = length(worldPos - gCBuf_EyePosition);
    float viewDistance = viewVal;
    float fogFactor = (FOG_END_LEVEL_Y - viewDistance) / (FOG_END_LEVEL_Y - FOG_START_LEVEL_Y);
    fogFactor = saturate(fogFactor);
    float heightFactor = exp(-FOG_DENSITY * height);
        //float finalFogFactor = fogFactor * heightFactor;
    float finalFogFactor = saturate(fogFactor * heightFactor / 3.0f);
    res.Output = lerp(originalColor, staticFogColor, finalFogFactor);
    
    return res;
}