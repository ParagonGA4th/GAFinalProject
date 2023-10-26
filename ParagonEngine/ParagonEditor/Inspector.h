#pragma once
#include "IEditorWindow.h"

namespace Pg::Editor::Window
{
	class Inspector : public IEditorWindow
	{
	public:
		Inspector();
		~Inspector();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;

		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;
		
		virtual void ShowWindow() override;

	private:
		bool _isShow;
	};
}


