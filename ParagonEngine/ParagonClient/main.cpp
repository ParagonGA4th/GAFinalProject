#include "Application.h"
#include <windows.h>

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR	  lpCmdLine,
	_In_ int		  nCmdShow)
{
	Pg::Client::Core::Application app;
	app.Run();
}