#include "Editor.h"
#include "EditorAction.h"

namespace Ed = Pg::Editor;


Ed::Editor::Editor()
{
	_action = std::make_unique<Pg::Editor::Core::EditorAction>();
	_action->Initialize();
}

Ed::Editor::~Editor()
{
	_action->Finalize();
}

void Ed::Editor::Run()
{
	_action->Loop();
}
