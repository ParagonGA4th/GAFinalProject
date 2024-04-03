#pragma once
#include "IEditorWindow.h"
#include <memory>
#include <vector>

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

		virtual std::string GetWindowName() override;

		virtual void SetDisable(bool disable) override;

	private:
		void SetData(void* data);
		void AddComponent();
		void ModifiedObject();

	private:
		/// inspector value
		std::string _winName;
		bool _isShow;
		bool _isDisable;	

		std::vector<Pg::Data::GameObject*> _modifiedObjList;
		int _flag;

		/// helper class
		Pg::UI::Manager::UIManager* _uiManager;
		std::unique_ptr<Pg::Editor::Window::InspectorHelper> _insHelper;

		/// Data value
		Pg::Data::GameObject* _selectGameObject;

	};
}


