#pragma once

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
	};
}

