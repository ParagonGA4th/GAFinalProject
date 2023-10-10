#pragma once
#include "UIRenderer.h"

#include <functional>

/// <summary>
/// 23.10.10 오수안
/// 버튼을 위한 클래스. 
/// </summary>

namespace Pg::Data
{
	class GameObject;

	class Button : public UIRenderer
	{
	public:
		Button(GameObject* obj);

	private:
		std::function<void()> _onClickEvent;

	};
}


