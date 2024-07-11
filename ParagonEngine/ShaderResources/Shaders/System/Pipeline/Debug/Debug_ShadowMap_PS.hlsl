
#include "../../../Appends/Libraries/Appends_PSCommon.hlsli"
#include "../../../Appends/Libraries/MathFunctions/Appends_ShadowFunctions.hlsli"
//#include "../../../Appends/Libraries/TextureBuffers/Appends_InstancedValues.hlsli"

POutQuad main(VOutQuad pin)
{
    POutQuad res;
    
    //DebugRender ¶ç¿ì±â.
    float2 tActualUV = pin.UV;
    float depthValue = GlobalShadowDepth_DSV_MainLight.Sample(fullScreenQuadSS, tActualUV);
    res.Output = float4(depthValue, depthValue, depthValue, 1.0f);
    
    //float3 tActualUV = float3(pin.UV, 0.f);
    //float3 tLightmap = internal_LightmapArray.Sample(fullScreenQuadSS, tActualUV);
    //res.Output = float4(tLightmap, 1.0f);
    return res;
}