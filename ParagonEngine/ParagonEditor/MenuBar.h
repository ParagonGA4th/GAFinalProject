#include "IEditorWindow.h"
#include <string>
#include <memory>
#include <map>

namespace Pg::Editor { class Event; }
namespace Pg::UI { class WidgetContainer; }
namespace Pg::UI::Manager { class UIManager; }

namespace Pg::Editor::Window
{
	class MenuBar : public IEditorWindow
	{
	public:
		MenuBar();
		~MenuBar();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;

		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;

	private:
		/// MenuBar value
		std::string _winName;
		bool _isShow;

		/// helper class
		Pg::UI::Manager::UIManager* _uiManager;
		Pg::UI::WidgetContainer* cons;

		//std::unique_ptr<Pg::Editor::Event> _changeObjectData;

		/// Data value
		std::map<std::string, std::map<std::string, bool*>> _menuBars;
		bool _isOpenScene;
		bool _isSaveScene;
	};
}