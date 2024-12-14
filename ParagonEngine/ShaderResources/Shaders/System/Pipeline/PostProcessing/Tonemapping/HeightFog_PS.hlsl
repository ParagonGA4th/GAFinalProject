//필요한 요소만 골라서 일차적으로 렌더링 수행.

#include "../../../Libraries/System_QuadLayouts.hlsli"
#include "../../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"
#include "../../../../Appends/Libraries/TextureBuffers/Appends_GBufferTextures.hlsli"

Texture2D<float4> QuadRTV : register(t5);

static const float FOG_START_LEVEL_Y = -100.0f;
static const float FOG_END_LEVEL_Y = -150.0f;
static const float FOG_DENSITY = 0.05f;
//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    
    //자체를 ToGBuffer에게 전달할 의도밖에 없다. UV 변환을 두번 거칠 필요가 없다.
    //이거 이제 개별적으로 적용 => 다른 용도로 사용해야!
    float4 originalColor = float4(QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz, 1.0f);
    
    //만약 Height이 일정 수준보다 낮으면 : Cubemap 값이 이미 들어오고 / 이를 섞어서 넘길 수 있을 것이다.
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