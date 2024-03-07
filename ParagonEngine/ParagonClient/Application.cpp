#include "Application.h"
#include "../ParagonEditor/IEditor.h"
#include "../ParagonEditor/Editor.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonEditor.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonEditor.lib")
#endif // _DEBUG

Pg::Client::Core::Application::Application()
{
	_editor = new Pg::Editor::Editor();
	
}

Pg::Client::Core::Application::~Application()
{
}

void Pg::Client::Core::Application::Run()
{
	_editor->Run();
}
