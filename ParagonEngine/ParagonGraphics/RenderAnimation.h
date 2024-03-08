#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/ParagonDefines.h"

/// <summary>
/// .fbx 파일들로, Animation에 1대1 대응하는 GraphicsResource 클래스이다.
/// </summary>

namespace Pg::Graphics
{
	namespace Helper
	{
		class AssimpBufferParser;
	}
	namespace Loader
	{
		class AssetBasic3DLoader;
	}
}

namespace Pg::Graphics
{
	class RenderAnimation : public Pg::Data::Resources::GraphicsResource
	{
		friend class Pg::Graphics::Helper::AssimpBufferParser;
		friend class Pg::Graphics::Loader::AssetBasic3DLoader;
	public:
		RenderAnimation(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		virtual ~RenderAnimation();

	public:
		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

	};
}



