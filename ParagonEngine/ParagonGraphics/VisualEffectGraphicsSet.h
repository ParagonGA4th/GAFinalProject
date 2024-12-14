#pragma once
#include "RenderTexture2D.h"
#include "RenderVertexShader.h"
#include "RenderPixelShader.h"
#include "BaseCustomEffect.h"
#include "SpriteEffect2D.h"

#include <dxtk/SpriteBatch.h>
#include <dxtk/GeometricPrimitive.h>
#include <dxtk/Effects.h>
#include <d3d11.h>

#include <vector>

namespace Pg::Graphics
{
	struct VisualEffectGraphicsSet
	{
		unsigned int _textureSize{ 0 };
		std::vector<RenderTexture2D*> _renderTextureVec;
		RenderVertexShader* _customRenderVertexShader{ nullptr };
		RenderPixelShader* _customRenderPixelShader{ nullptr }; 
		ID3D11BlendState* _customBlendState{ nullptr };

		//2D Space Rendering일 경우, 활용.
		//2DSpace : DX11::SpriteBatch가 Begin에 등록해줘야 한다.
		std::unique_ptr<DirectX::SpriteBatch> _spriteBatch{ nullptr };
		std::unique_ptr<SpriteEffect2D> _spriteEffect2D{ nullptr };
			
		ID3D11InputLayout* _inputLayout{ nullptr }; //공동 활용.

		//3D Space Rendering일 경우, 활용. GeometryGenerator의 _QUAD_VB,IB와 같이.
		//3DSpace : DX11:	:BasicEffect같은 요소들이 달라져야 한다. 등록시.
		//GeometricPrimitive는 안 쓴다 -> SetVS PS 명시적으로 호출할 것.
		//Apply / GetVertexShaderBytecode만 구현하면 활용 가능.
		//https://github.com/microsoft/DirectXTK/wiki/Authoring-an-Effect
		std::unique_ptr<DirectX::IEffect> _effect3D{ nullptr };
		//SetMatrix 접근 + Runtime DynamicCast의 비용 피하기 위해 별도로 포인터 보관.
		DirectX::IEffectMatrices* _effectStoreMatrixForm{ nullptr };
		//만약 Custom이면 해당 포인터가 Nullptr가 아닐 것이다.
		BaseCustomEffect* _baseCustomEffect{ nullptr };
		//만약 BasicEffect라면, 해당 포인터가 채워져 있을 것이다.
		DirectX::BasicEffect* _dxtkBasicEffect{ nullptr };
		//BasicEffect만 시간 상 3D plane effect에 활용할 것이다.
		bool _isBillboardAnimation{ false };
		unsigned int _currentTextureFrame{ 0 };
		float _recordedTime{ 0.f };
		float _changeBaseTime{ 0.25f }; //이건 나중에 Serialize할 수 있을 수도?
	};
}
