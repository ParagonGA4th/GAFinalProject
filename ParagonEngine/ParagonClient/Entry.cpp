#define WIN32_LEAN_AND_MEAN 
#include "Resource.h"
#include "../ParagonCore/CoreMain.h"
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <memory>
#include <tchar.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonCore.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonCore.lib")
#endif // _DEBUG

///Paragon Engine의 Entry Point. 

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR	  lpCmdLine,
	_In_ int		  nCmdShow)
{

	//테스트용 초기화(유니크 포인터)
	std::unique_ptr<Pg::Core::CoreMain> main;
	main = std::make_unique<Pg::Core::CoreMain>();
	main->Initialize(static_cast<void*>(hInstance), nCmdShow);


	main->Update();

	main->Finalize();
}

