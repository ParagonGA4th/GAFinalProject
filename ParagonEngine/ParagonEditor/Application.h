#pragma once
#include "IEditor.h"

namespace Pg::Editor::Main 
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

	private:
		Pg::Editor::IEditor* _editor;
	};
}

