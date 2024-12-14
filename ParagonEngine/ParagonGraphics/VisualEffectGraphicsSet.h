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

		//2D Space Rendering�� ���, Ȱ��.
		//2DSpace : DX11::SpriteBatch�� Begin�� �������� �Ѵ�.
		std::unique_ptr<DirectX::SpriteBatch> _spriteBatch{ nullptr };
		std::unique_ptr<SpriteEffect2D> _spriteEffect2D{ nullptr };
			
		ID3D11InputLayout* _inputLayout{ nullptr }; //���� Ȱ��.

		//3D Space Rendering�� ���, Ȱ��. GeometryGenerator�� _QUAD_VB,IB�� ����.
		//3DSpace : DX11:	:BasicEffect���� ��ҵ��� �޶����� �Ѵ�. ��Ͻ�.
		//GeometricPrimitive�� �� ���� -> SetVS PS ��������� ȣ���� ��.
		//Apply / GetVertexShaderBytecode�� �����ϸ� Ȱ�� ����.
		//https://github.com/microsoft/DirectXTK/wiki/Authoring-an-Effect
		std::unique_ptr<DirectX::IEffect> _effect3D{ nullptr };
		//SetMatrix ���� + Runtime DynamicCast�� ��� ���ϱ� ���� ������ ������ ����.
		DirectX::IEffectMatrices* _effectStoreMatrixForm{ nullptr };
		//���� Custom�̸� �ش� �����Ͱ� Nullptr�� �ƴ� ���̴�.
		BaseCustomEffect* _baseCustomEffect{ nullptr };
		//���� BasicEffect���, �ش� �����Ͱ� ä���� ���� ���̴�.
		DirectX::BasicEffect* _dxtkBasicEffect{ nullptr };
		//BasicEffect�� �ð� �� 3D plane effect�� Ȱ���� ���̴�.
		bool _isBillboardAnimation{ false };
		unsigned int _currentTextureFrame{ 0 };
		float _recordedTime{ 0.f };
		float _changeBaseTime{ 0.25f }; //�̰� ���߿� Serialize�� �� ���� ����?
	};
}
