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

		//Texture 관련 매커닉.
		//virtual void SetTexture(unsigned int index, ID3D11ShaderResourceView*& value) override;
		//virtual unsigned int GetTextureCount() override;

		//일부 하드코딩이 있더라도, 일단은 게임을 내자. Custom Parameter NO, 이미 올라간 거에서.
		void SetCustomShaderInfo();

		

	private:

		VERenderingSet* _veSet;
		LowDX11Storage* _DXStorage{ nullptr };
		//AnimatedTexture / Scrolling Background에 영향을 받지 않고 그냥 렌더를 할 것인지/
		// 아니면 영향 받는지.
		bool _isDefaultRenderMode{ true };
		std::unique_ptr<AnimatedTexture> _animatedTexture = nullptr;
		std::unique_ptr<ScrollingBackground> _scrollingBackground = nullptr;
	};
}