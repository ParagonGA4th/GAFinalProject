#pragma once
#include "RendererBase2D.h"
#include "../ParagonMath/PgMath.h"

#include <string>

namespace Pg::Data
{
	using namespace Pg::Math;

	class GameObject;

	class TextRenderer : public RendererBase2D
	{
	public:
		TextRenderer(GameObject* obj);

		//구체화된 Render 함수, 호출되면 실제 기능 수행해야!
		virtual void Render() override;

	public:
		void SetString(std::string str);
		std::string GetString();

		void SetSize(float size);
		float GetSize();

		void SetFont(std::string fontName);
		std::string GetFontName();

		void SetFontColor(PGFLOAT4 color);
		PGFLOAT4 GetFontColor();

	private:
		std::string _string;
		std::string _font;
		PGFLOAT4 _fontColor;
		float _fontSize;
	};

}


