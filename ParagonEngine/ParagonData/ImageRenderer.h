#pragma once
#include "RendererBase2D.h"

#include <string>
#include <visit_struct/visit_struct.hpp>

/// <summary>
/// 23.10.10 오수안
/// 각종 스프라이트 이미지와 빈 패널을 위한 컴포넌트 클래스
/// </summary>

namespace Pg::Data
{
	class GameObject;

	class ImageRenderer : public RendererBase2D
	{
	public:
		ImageRenderer(GameObject* obj);

		virtual void Internal_EngineAwake() override;

		virtual void OnSerialize(SerializeVector& sv);
		virtual void OnDeserialize(SerializeVector& sv);

		void SetImagePath(std::string path);
		std::string GetImagePath();

		//100.f 가 가장 최대.
		float _fillRatio = 100.f;

	public:
		BEGIN_VISITABLES(ImageRenderer);
		VISITABLE(std::string, _imagePath); // 필수 조건
		END_VISITABLES;
	};
}



