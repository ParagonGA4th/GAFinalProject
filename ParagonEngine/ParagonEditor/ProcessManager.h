#pragma  once
#include <windows.h>
#include <memory>

namespace Pg::Core { class ProcessMain; }
namespace Pg::API::Input { class PgInput; }
namespace Pg::Editor::Helper{ class EditorHelper; }

namespace Pg::Editor::Manager
{
	class ProcessManager
	{
	public:
		ProcessManager();
		~ProcessManager();

		void Initialize(void* hWnd, float screenWidth, float screenHeight);
		void Update();
		void LastUpdate();
		void Finalize();

		void ProcessHandler(MSG message);
	private:
		std::unique_ptr<Pg::Core::ProcessMain> _coreMain;
		Pg::API::Input::PgInput* _input;
		Pg::Editor::Helper::EditorHelper* _edHepler;

		bool _isCoreInitailized;
	};
}

