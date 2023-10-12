#include "Button.h"
#include "ImageRenderer.h"
#include "TextRenderer.h"

#include "GameObject.h"

namespace Pg::Data
{
	class GameObject;

	Button::Button(GameObject* obj)
		:Component(obj)
	{
		obj->AddComponent<ImageRenderer>();
		obj->AddComponent<TextRenderer>();
	}
}
