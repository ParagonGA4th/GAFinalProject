#pragma  once
#include "IEditorManager.h"
#include "../ParagonData/EditorMode.h"
#include <memory>

namespace Pg::Core { class ProcessMain; }
namespace Pg::API::Input { class PgInput; }
namespace Pg::Editor::Data{ class DataContainer; }

namespace Pg::Editor::Manager
{
	class ProcessManager : public IEditorManager
	{
	public:
		ProcessManager(float width, float height);
		~ProcessManager();

		virtual void Initialize(void* hWnd) override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Finalize() override;

		virtual void ManagerHandler(MSG message) override;

		void SetEditorMode(void* mode); // Event Ã³¸®¿ë
		void SetEditorMode(Pg::Data::Enums::eEditorMode mode);

	private:
		std::unique_ptr<Pg::Core::ProcessMain> _coreMain;
		Pg::API::Input::PgInput* _input;
		Pg::Editor::Data::DataContainer* _dataContainer;

		float _screenWidth;
		float _screenHeight;
		bool _isCoreInit;
	};
}