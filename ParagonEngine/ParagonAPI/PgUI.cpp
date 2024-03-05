#include "PgUI.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

namespace Pg::API::UI
{
	void PgUI::Initialize()
	{

	}

	void PgUI::SetOnClickEvent(std::function<void()> onClickEvent)
	{
		_button->SetOnClickEvent(onClickEvent);
	}

	void PgUI::Click()
	{
		_button->Click();
	}
}