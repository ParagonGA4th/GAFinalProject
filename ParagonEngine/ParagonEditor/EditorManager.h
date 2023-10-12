#pragma once
#include <memory>
#include <gainput/gainput.h>

namespace Pg::Core
{
	class ProcessMain;
}

namespace Pg::API::Input
{
	class PgInput;
}

namespace Pg::Editor::Manager
{
	class EditorManager
	{
	public:
		EditorManager();
		~EditorManager();
		
		void Initialize(void* hWnd, float width, float height);
		void Update();

		// Graphics 관련, 화면의 크기가 재정의 되었을 때 사용되는 함수
		void ReSizeHandler(float width, float height);

		void InputHandler(MSG message);
		
		void Finalize();

	private:
		std::unique_ptr<Pg::Core::ProcessMain> _coreMain;
		Pg::API::Input::PgInput* _input;

	private:
		bool _isCoreInitailized;
	};
}


