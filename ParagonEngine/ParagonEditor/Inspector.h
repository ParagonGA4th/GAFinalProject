#pragma once
#include "IEditorWindow.h"
#include <string>
#include <memory>

namespace Pg::Editor { class Event; }
namespace Pg::Data { class GameObject; }
namespace Pg::UI::Manager { class UIManager; }
namespace Pg::UI { class WidgetContainer; }

namespace Pg::Editor::Window
{
	class InspectorHelper;
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

	private:
		void SetData(void* data);

	private:
		/// inspector value
		std::string _winName;
		bool _isShow;

		/// helper class
		Pg::UI::Manager::UIManager* _uiManager;
		std::unique_ptr<Pg::UI::WidgetContainer> _widgetCon;
		std::unique_ptr<Pg::Editor::Window::InspectorHelper> _insHelper;

		/// Data value
		Pg::Data::GameObject* _selectGameObject;
		bool* _isAddComponent;
	};
}


