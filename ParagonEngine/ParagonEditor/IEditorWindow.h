#pragma once
#include <string>

namespace Pg::Editor::Window
{
	class IEditorWindow
	{
	public:
		virtual void Initialize() abstract;
		virtual void Update() abstract;
		virtual void Finalize() abstract;

		virtual void SetShow(bool show) abstract;
		virtual bool GetShow() abstract;

		virtual std::string GetWindowName() abstract;

		virtual void SetDisable(bool disable) abstract;
	};
}

