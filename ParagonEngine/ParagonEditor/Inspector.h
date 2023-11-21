#pragma once
#include "IEditorWindow.h"
#include <string>

namespace Pg::Editor::Data { class DataContainer; }
namespace Pg::UI { class WidgetContainer; }
namespace Pg::UI::Manager { class UIManager; }

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

	private:

	private:
		/// inspector value
		std::string _winName;
		bool _isShow;

		/// helper class
		Pg::Editor::Data::DataContainer* _dataContainer;
		Pg::UI::Manager::UIManager* _uiManager;
		Pg::UI::WidgetContainer* cons;

		/// Data value
		std::string _objName;
		std::string _objTag;
		bool _isObjActive;

		float _position[3] = { 0.1f, 0.1f, 0.1f };
		float _rotation[3] = { 0.1f, 0.1f, 0.1f };
		float _scale[3] = { 0.1f, 0.1f, 0.1f };
	};
}


