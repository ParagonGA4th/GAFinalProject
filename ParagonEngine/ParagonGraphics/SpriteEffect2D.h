#pragma once
#include "BaseCustomEffect.h"
#include "AnimatedTexture.h"
#include "ScrollingBackground.h"

#include <memory>

namespace Pg::Graphics
{
	class VisualEffectRenderer;
	class LowDX11Storage;

	class SpriteEffect2D
	{
		friend class VisualEffectRenderer;
	public:
		SpriteEffect2D(VERenderingSet* veSet);

		//Texture ���� ��Ŀ��.
		//virtual void SetTexture(unsigned int index, ID3D11ShaderResourceView*& value) override;
		//virtual unsigned int GetTextureCount() override;

		//�Ϻ� �ϵ��ڵ��� �ִ���, �ϴ��� ������ ����. Custom Parameter NO, �̹� �ö� �ſ���.
		void SetCustomShaderInfo();

		

	private:

		VERenderingSet* _veSet;
		LowDX11Storage* _DXStorage{ nullptr };
		//AnimatedTexture / Scrolling Background�� ������ ���� �ʰ� �׳� ������ �� ������/
		// �ƴϸ� ���� �޴���.
		bool _isDefaultRenderMode{ true };
		std::unique_ptr<AnimatedTexture> _animatedTexture = nullptr;
		std::unique_ptr<ScrollingBackground> _scrollingBackground = nullptr;
	};
}