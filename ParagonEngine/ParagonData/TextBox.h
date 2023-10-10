#pragma once
#include "UIRenderer.h"

#include <string>

namespace Pg::Data
{
	class GameObject;

	class TextBox : public UIRenderer
	{
	public:
		TextBox(GameObject* obj);

	public:
		void SetString(std::string str);
		std::string GetString();

		void SetSize(float size);
		float GetSize();

	private:
		std::string _string;
		float _fontSize;
	};

}


