#pragma once
#include "RenderTexture2D.h"
#include "RenderVertexShader.h"
#include "RenderPixelShader.h"

#include <dxtk/SpriteBatch.h>
#include <dxtk/GeometricPrimitive.h>
#include <dxtk/Effects.h>
#include <vector>

namespace Pg::Graphics
{
	struct VisualEffectGraphicsSet
	{
		unsigned int _textureSize{ 0 };
		std::vector<RenderTexture2D*> _renderTextureVec{ nullptr };
		RenderVertexShader* _customRenderVertexShader{ nullptr };
		RenderPixelShader* _customRenderPixelShader{ nullptr }; 

		//2D Space Rendering일 경우, 활용.
		//2DSpace : DX11::SpriteBatch가 Begin에 등록해줘야 한다.
		std::unique_ptr<DirectX::SpriteBatch> _spriteBatch{ nullptr };

		//3D Space Rendering일 경우, 활용. GeometryGenerator의 _QUAD_VB,IB와 같이.
		//3DSpace : DX11:	:BasicEffect같은 요소들이 달라져야 한다. 등록시.
		//GeometricPrimitive는 안 쓴다 -> SetVS PS 명시적으로 호출할 것.
		//Apply / GetVertexShaderBytecode만 구현하면 활용 가능.
		//https://github.com/microsoft/DirectXTK/wiki/Authoring-an-Effect
		std::unique_ptr<DirectX::IEffect> _effect3D{ nullptr };
		ID3D11InputLayout* _inputLayout3D{ nullptr };
	};
}
