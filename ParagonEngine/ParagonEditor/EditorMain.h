#pragma once
#include "IEditor.h"
#include "EditorDLLExporter.h"
#include "../ParagonProcess/ProcessMain.h"
#include <memory>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonProcess.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonProcess.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

namespace Pg::Core
{
	class ProcessMain;
}

class ImGuiManager;
class FileManager;

namespace Pg::Editor
{
	class EditorMain : public IEditor 
	{
	public:
		PARAGON_EDITOR_DLL EditorMain();
		~EditorMain();

		virtual PARAGON_EDITOR_DLL void Initialize(void* hWnd, float width, float height) override;
		virtual PARAGON_EDITOR_DLL void Update() override;
		virtual PARAGON_EDITOR_DLL void Finalize() override;

	private:
		std::unique_ptr<ImGuiManager> _imGuiManager;
		std::unique_ptr<FileManager> _fileManager;

		std::unique_ptr<Pg::Core::ProcessMain> _coreMain;

	private:
		/// 윈도우 크기 변경시 코어의 OnWindowResized()를 호출하도록 하기 위한 변수들
		// WndProc 접근을 위한 스태틱 변수
		static Pg::Core::ProcessMain* _coreMainStatic;

		// 코어의 Initialize 이후에 스태틱 변수에 접근하도록 하기 위한 bool 변수
		static bool _isCoreInitialized;

		// TODO: 그저 WndProc에 접근하고 싶었을 뿐인데 쓸데없이 복잡하다. 개선이 필요하다.
		// WndProc은 프로그램 시작 시점부터 즉, 코어의 초기화 이전에 호출될 수 있다.
		// 1) 만일 스태틱 변수의 초기화를 에디터의 Initialize에서 하면 그 전까지 스태틱 변수가 nullptr가 되고,
		//    WndProc에서 이에 접근하여 런타임 오류가 발생한다.
		// 2) 만일 스태틱 변수의 초기화를 에디터의 생성자에서 하면 코어가 초기화되지 않은 상태로 OnWindowResized()를
		//    호출하여 문제가 발생한다.
		// 3) 이를 위해 bool 변수를 도입하였는데 두 가지 문제점이 있다.
		// 4) 첫째, 비 직관적이다.
		// 5) 둘째, 불필요한 조건문 비교가 수행된다.
		// 6) 따라서 개선이 필요하다.
		// 2023. 09. 14. 김정식

	private:
		Pg::API::Input::PgInput* _input;
	};

}

