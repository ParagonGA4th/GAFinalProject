#pragma once
#include "IEditorManager.h"
#include <memory>

namespace Pg::Editor::Data { class DataContainer; }
namespace Pg::Editor::Manager
{
	class WindowManager;
	class EditorManager : public IEditorManager
	{
	public:
		EditorManager();
		~EditorManager();
		
		virtual void Initialize(void* hWnd) override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Finalize() override;

		virtual void ManagerHandler(MSG message) override;

	private:
		Pg::Editor::Data::DataContainer* _dataContainer;
		std::unique_ptr<Pg::Editor::Manager::WindowManager> _windowManager;
	};
}