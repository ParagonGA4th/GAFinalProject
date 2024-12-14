#pragma once
#include "IEditorWindow.h"
#include <memory>

namespace Pg::Editor { class Event; }
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

		virtual std::string GetWindowName() override;

		virtual void SetDisable(bool disable) override;

	private:
		void DataSet(float x, float y, float titleBarHeight);
	
	private:
		/// Scene Value
		std::string _winName;
		bool _isShow;
		bool _isDisable;

		/// helper Class
		Pg::Editor::Data::DataContainer* _dataContainer;
		Pg::UI::Manager::UIManager* _uiManager;
		std::unique_ptr<Pg::UI::WidgetContainer> _widgetCon;
		std::unique_ptr<Pg::Editor::Event> _mousePos;

		/// Data Value
		float* _imageWidth;
		float* _imageHeight;

	};
}

