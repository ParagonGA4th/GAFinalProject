#pragma once
#include "../ParagonCore/CoreMain.h"
#include <windows.h>
#include <memory>

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonCore.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonCore.lib")
#endif // _DEBUG

namespace Pg::Core
{
	class CoreMain;
}

class ImGuiManager;
class FileManager;

class EditorMain
{
public:
	EditorMain();
	~EditorMain();

	long Initialize(void* hInstance, int cmdShow);
	void Update();
	void Finalize();

private:
	ATOM RegisterClass(HINSTANCE hInstance);
	BOOL CreateWindows(HINSTANCE hInstance, int cmdShow);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	//윈도우 관련 변수들
	HWND _hWnd;
	MSG _msg;
	int _screenWidth;
	int _screenHeight;

	const WCHAR* _className;
	const WCHAR* _windowName;

private:
	std::unique_ptr<ImGuiManager> _imGuiManager;
	std::unique_ptr<FileManager> _fileManager;
	
	std::unique_ptr<Pg::Core::CoreMain> _coreMain;
	static Pg::Core::CoreMain* _coreMainStatic;
	static bool _isCoreInitialized;
};

