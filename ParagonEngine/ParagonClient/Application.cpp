#include "Application.h"
#include "../ParagonEditor/IEditor.h"
#include "../ParagonEditor/Editor.h"

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
