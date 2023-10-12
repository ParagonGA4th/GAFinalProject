#include "Editor.h"
#include "EditorMain.h"

namespace Ed = Pg::Editor;


Ed::Editor::Editor()
{
	_main = std::make_unique<Pg::Editor::Core::EditorMain>();
	_main->Initialize();
}

Ed::Editor::~Editor()
{
	_main->Finalize();
}

void Ed::Editor::Run()
{
	_main->Loop();
}
