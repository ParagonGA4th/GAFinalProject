#pragma once
#include <windows.h>
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
	class UIManager;

	class EditorManager
	{
	public:
		EditorManager();
		~EditorManager();
		
		void Initialize(HWND hWnd, float width, float height);
		void Update();

		// Graphics 관련, 화면의 크기가 재정의 되었을 때 사용되는 함수
		//void ReSizeHandler(float width, float height);

		// Input 관련, 입력 받기 위해 window의 message를 받아야함
		//void InputHandler(MSG message);

		// Imgui 관련, 입력 등 관련 기능을 사용하려면 WndProc에 있어야 함
		void Handler(MSG message);
		
		void Finalize();

	private:
		std::unique_ptr<Pg::Core::ProcessMain> _coreMain;
		Pg::API::Input::PgInput* _input;

		std::unique_ptr<Pg::Editor::Manager::UIManager> _UIManager;

	private:
		bool _isCoreInitailized;
	};
}


