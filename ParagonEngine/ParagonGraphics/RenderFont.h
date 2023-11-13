#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"

#include <memory>
#include <string>
#include <dxtk/SpriteFont.h>

/// <summary>
/// 실제 Font와 1대1 대응되는 클래스. 게임엔진에서 저장된 Font와 연동되어 보관된다.
/// -> Text Rendering에 사용된다.
/// </summary>

namespace Pg::Graphics
{
	class RenderFont : public Pg::Data::Resources::GraphicsResource
	{
	public:
		RenderFont(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderFont();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		std::unique_ptr<DirectX::SpriteFont> _font;
	};
}


