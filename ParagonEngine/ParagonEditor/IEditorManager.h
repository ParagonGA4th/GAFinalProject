#pragma once
#include <windows.h>

namespace Pg::Editor::Manager
{
	class IEditorManager
	{
	public:
		virtual void Initialize(void* hWnd) abstract;
		virtual void Update() abstract;
		virtual void LateUpdate() abstract;
		virtual void Finalize() abstract;

		virtual void ManagerHandler(MSG message) abstract;
	};
}