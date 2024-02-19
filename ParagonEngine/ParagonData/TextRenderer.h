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
		enum { MAXIMUM_TEXTBUFFER_COUNT = 1024 };
	public:
		TextRenderer(GameObject* obj);


	public:
		void SetString(const std::string& str);
		std::string GetString();
		//const wchar_t* GetWCharPointer() const;

		void SetSize(float size);
		float GetSize();

		void SetFont(std::string fontName);
		std::string GetFontName();

		void SetFontColor(PGFLOAT4 color);
		PGFLOAT4 GetFontColor();

		PGFLOAT4 _fontColor;
		std::string _string;
		std::string _font;
		
		float _fontSize;

	private:
		//wchar_t _wBuffer[MAXIMUM_TEXTBUFFER_COUNT];
	};

}


