#pragma once

namespace Pg::Editor
{
	class IEditor;
}

namespace Pg::Client::Core
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

