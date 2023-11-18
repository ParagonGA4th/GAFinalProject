#pragma once
#include "IEditorWindow.h"
#include <string>

namespace Pg::Editor::Data { class DataContainer; }
namespace Pg::UI { class WidgetContainer; }
namespace Pg::UI::Manager { class UIManager; }

namespace Pg::Editor::Window
{
	class Scene : public IEditorWindow
	{
	public:
		Scene();
		~Scene();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;
		

		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;
	
	private:
		/// Scene Value
		std::string _winName;
		bool _isShow;

		/// helper Class
		Pg::Editor::Data::DataContainer* _dataContainer;
		Pg::UI::Manager::UIManager* _uiManager;
		Pg::UI::WidgetContainer* cons;

		/// Data Value
	};
}

