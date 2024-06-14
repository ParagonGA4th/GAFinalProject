//Appends PS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"
#include "../../../Libraries/MathFunctions/Appends_ShadowFunctions.hlsli"
//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    //모든 Appends의 Pixel Shader 실행 전에, ClipUnfits 함수가 들어있어야 한다. 구조 특성 때문.
    ClipUnfits(pin.UV);
    
    //본격적인 Shader Code.
    POutQuad res;
    
    float2 tActualUV = GetUV_F2(pin.UV);
    float depthValue = GlobalShadowDepth_DSV_MainLight.Sample(fullScreenQuadSS, tActualUV);
    //float depthValue = 0.1f;
    res.Output = float4(depthValue, depthValue, depthValue, 1.0f);
    //float depthValue = GetDepth_WDivide(tActualUV);
   
    //CheckerBoard.
    //float2 tActualUV = GetUV_F2(pin.UV);
    //float2 uv = tActualUV * 10.0; // Adjust the scale of the checkerboard pattern 
    //float2 checker = floor(uv) % 2.0;
    //float isBlack = checker.x == checker.y;
    //
    //float4 black = float4(0.0, 0.0, 0.0, 1.0);
    //float4 white = float4(1.0, 1.0, 1.0, 1.0);
    //
    //res.Output = lerp(white, black, isBlack);
    
    //Spiral
    //float2 tActualUV = GetUV_F2(pin.UV);
    //float2 uv = tActualUV * 2.0 - 1.0; // Normalize UV coordinates to [-1, 1] range
    //float angle = atan2(uv.y, uv.x); // Calculate the angle from the center
    //float radius = length(uv); // Calculate the distance from the center
    //float spiral = frac(angle / (2.0 * 3.14159265) + radius * 0.5); // Calculate the spiral value
    //float3 color1 = float3(1.0, 0.0, 0.0); // Red color
    //float3 color2 = float3(0.0, 1.0, 0.0); // Green color
    //float3 finalColor = lerp(color1, color2, spiral); // Interpolate between colors based on the spiral value
    //
    //res.Output = float4(finalColor, 1.0);
    
    return res;
}