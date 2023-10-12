#pragma once
#include "IEditor.h"
#include "EditorDLLExporter.h"
#include <memory>

namespace Pg::Editor
{
	namespace Core
	{
		class EditorMain;
	}

	class Editor:public IEditor
	{
	public:
		PARAGON_EDITOR_DLL Editor();
		~Editor();

		virtual PARAGON_EDITOR_DLL void Run() override;
	private:
		std::unique_ptr<Pg::Editor::Core::EditorMain> _main;
	};
}



