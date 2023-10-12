#include "Application.h"
#include "EditorMain.h"

Pg::Editor::Main::Application::Application()
{
	_editor = new Pg::Editor::EditorMain();
	_editor->Initialize();
}

Pg::Editor::Main::Application::~Application()
{
	_editor->Finalize();
}

void Pg::Editor::Main::Application::Run()
{
	_editor->Run();
}
