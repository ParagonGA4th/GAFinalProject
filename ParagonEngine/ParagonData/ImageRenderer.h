#pragma once
#include "RendererBase2D.h"

#include <string>

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

		void SetImagePath(std::string path);
		std::string GetImagePath();

	private:
		std::string _imagePath;
	};
}



