#pragma once
#include "IEditorWindow.h"
#include <memory>

namespace Pg::Editor { class Event; }
namespace Pg::UI { class WidgetContainer; }
namespace Pg::UI::Manager { class UIManager; }

namespace Pg::Editor::Window
{
	class Layout : public IEditorWindow
	{
	public:
		Layout();
		~Layout();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;

		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;

		virtual std::string GetWindowName() override;

		virtual void SetDisable(bool disable) override;

	private:
		void FileUpdate();

	private:
		/// Layout value
		std::string _winName;
		bool _isShow;

		/// helper class
		Pg::UI::Manager::UIManager* _uiManager;
		std::unique_ptr<Pg::UI::WidgetContainer> _widgetCon;
		std::unique_ptr<Pg::Editor::Event> _fileState;

		bool _isNewScene;
		bool _isOpenScene;
		bool _isSaveScene;
		bool _isNewProj;
		bool _isOpenProj;
		bool _isSaveProj;
	};
}

