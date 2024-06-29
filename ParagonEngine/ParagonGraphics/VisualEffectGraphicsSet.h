#pragma once
#include "RenderTexture2D.h"
#include "RenderVertexShader.h"
#include "RenderPixelShader.h"

namespace Pg::Graphics
{
	struct VisualEffectGraphicsSet2D
	{
		RenderTexture2D* _renderTexture{ nullptr };
		RenderVertexShader* _customRenderVertexShader{ nullptr };
		RenderPixelShader* _customRenderPixelShader{ nullptr };
	};

	struct VisualEffectGraphicsSet3D
	{
		RenderTexture2D* _renderTexture{ nullptr };
		RenderVertexShader* _customRenderVertexShader{ nullptr };
		RenderPixelShader* _customRenderPixelShader{ nullptr };
	
		
	
	
	
	
	};

}
