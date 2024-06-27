#pragma once
#include "RendererBase2D.h"
#include "eFillRatioDirection.h"

#include <string>
#include <functional>
#include <visit_struct/visit_struct.hpp>

/// <summary>
/// 23.10.10 오수안
/// 각종 스프라이트 이미지와 빈 패널을 위한 컴포넌트 클래스
/// </summary>

namespace Pg::Graphics
{
	class RenderObjectImage2D;
}

namespace Pg::Data
{
	class GameObject;

	class ImageRenderer : public RendererBase2D
	{
		friend class Pg::Graphics::RenderObjectImage2D;
	public:
		ImageRenderer(GameObject* obj);

		virtual void Internal_EngineAwake() override;

		virtual void OnSerialize(SerializeVector& sv);
		virtual void OnDeserialize(SerializeVector& sv);

		void SetImagePath(std::string path);
		std::string GetImagePath();

		void SetFillRatio(float val);
		float GetFillRatio();

		void SetImageIndex(unsigned int val);
		unsigned int GetImageIndex(); // Defaults to 0

		void SetFillRatioDirection(eFillRatioDirection val);
		eFillRatioDirection GetFillRatioDirection();

	public:
		BEGIN_VISITABLES(ImageRenderer);
		VISITABLE(std::string, _imagePath); // 필수 조건
		END_VISITABLES;

	private:
		std::function<void(unsigned int)> _setImageIndexFunc;
		std::function<unsigned int(void)> _getImageIndexFunc;

		//100.f 가 가장 최대.
		float _fillRatio = 100.f;

		//FillRatio 채워지는 양상.
		eFillRatioDirection _fillRatioDirection{ eFillRatioDirection::LEFT_TO_RIGHT };
	};
}



