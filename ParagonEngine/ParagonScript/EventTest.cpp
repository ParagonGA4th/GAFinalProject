#include "EventTest.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Button.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonAPI/PgInput.h"
#include <singleton-cpp/singleton.h>
#include <cassert>

EventTest::EventTest(Pg::Data::GameObject* owner) :
	Script(owner)
{

}

void EventTest::Start()
{
	tButton = _object->GetComponent<Pg::Data::Button>();

	tButton->SetOnClickEvent([]
		{
			//assert(false, "너의 프로그램은 해킹 당했다. 으하하하하"); 
			PG_TRACE("Button Clicked!!");
		});
}

void EventTest::Update()
{
	using namespace Pg::API::Input;

	// gainput을 사용하여 비율 변환한 Click 처리
	const float mouseX = _pgInput->GetMouseX();
	const float mouseY = _pgInput->GetMouseY();

	const float screenWidth = 1920;
	const float screenHeight = 1080;

	const float objectX = _object->_transform._position.x;
	const float objectY = _object->_transform._position.y;

	// 화면 비율에 따라 마우스 위치 변환
	const float scaledMouseX = mouseX * screenWidth;
	const float scaledMouseY = mouseY * screenHeight;

	if (scaledMouseX > (objectX - tButton->GetImageWidth() / 2.0f) &&
		scaledMouseX <  (objectX + tButton->GetImageWidth() / 2.0f) &&
		scaledMouseY >(objectY - tButton->GetImageHeight() / 2.0f) &&
		scaledMouseY < (objectY + tButton->GetImageHeight() / 2.0f))
	{
		if (_pgInput->GetKeyDown(MouseLeft))
		{
			tButton->Click();
		}
	}
}