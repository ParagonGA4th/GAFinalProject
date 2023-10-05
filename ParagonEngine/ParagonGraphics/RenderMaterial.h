#pragma once
#include "../ParagonData/GraphicsResource.h"

/// <summary>
/// 실제 Material과 1대1 대응되는 클래스. 게임엔진에서 저잗된 Material과 연동되어 보관된다.
/// </summary>

namespace Pg::Graphics
{
	class RenderMaterial : public Pg::Data::Resources::GraphicsResource
	{ 
	public:
		RenderMaterial(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderMaterial();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;
	};

}


