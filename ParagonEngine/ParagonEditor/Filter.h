#pragma once
#include "IEditorWindow.h"
#include <unordered_map>
#include <vector>
#include <memory>

namespace Pg::UI { class WidgetContainer; }
namespace Pg::UI::Manager { class UIManager; }
namespace Pg::Editor::Data { class DataContainer; }

namespace Pg::Editor::Window
{
	using FilterData = std::unordered_map<std::string, std::vector<std::string>>;

	class Filter : public IEditorWindow
	{
	public:
		Filter();
		~Filter();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Finalize() override;
		
		virtual void SetShow(bool show) override;
		virtual bool GetShow() override;

		virtual std::string GetWindowName() override;

		virtual void SetDisable(bool disable) override;

	private:
		void DataSet(std::string path);
		FilterData SeparateFiles(std::string path, std::string folderName);
		void FileSelected();

	private:
		/// filter Value
		std::string _winName;
		bool _isShow;

		/// helper class
		Pg::Editor::Data::DataContainer* _dataContainer;
		Pg::UI::Manager::UIManager* _uiManager;
		std::unique_ptr<Pg::UI::WidgetContainer> _widgetCon;

		/// Data Value
		std::unordered_map<std::string, std::vector<std::string>> _fileNames;
		std::string _selectObject;
		bool* _isDoubleClicked;
	};
}


