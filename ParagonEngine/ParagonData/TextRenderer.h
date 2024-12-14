#pragma once
#include "RendererBase2D.h"
#include "../ParagonMath/PgMath.h"

#include <string>
#include <visit_struct/visit_struct.hpp>

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

		virtual void OnSerialize(SerializeVector& sv);
		virtual void OnDeserialize(SerializeVector& sv);

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


		BEGIN_VISITABLES(TextRenderer);
		VISITABLE(PGFLOAT4, _fontColor); // 필수 조건
		VISITABLE(std::string, _font);  // 필수 조건
		VISITABLE(float, _fontSize); // 필수 조건
		END_VISITABLES;

		std::string _string;
	private:
		//wchar_t _wBuffer[MAXIMUM_TEXTBUFFER_COUNT];
	};

}


