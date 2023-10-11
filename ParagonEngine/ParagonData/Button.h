#pragma once
#include "RendererBase2D.h"

#include <functional>

/// <summary>
/// 23.10.10 오수안
/// 버튼을 위한 클래스. 
/// </summary>

namespace Pg::Data
{
	class GameObject;

	class Button : public RendererBase2D
	{
	public:
		Button(GameObject* obj);

	private:
		//TODO 나중에 필요하면 템플릿으로 바꿀 것
		std::function<void()> _onClickEvent;

	};
}


