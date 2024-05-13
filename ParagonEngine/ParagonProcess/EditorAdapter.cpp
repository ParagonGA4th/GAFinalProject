#include "EditorAdapter.h"
#include "ProcessMain.h"
#include "EngineGraphicsAdapter.h"

namespace Pg::Core
{
	EditorAdapter::EditorAdapter(ProcessMain* core)
		: _processMain(core)
	{
	}

	EditorAdapter::~EditorAdapter()
	{
	}

	void EditorAdapter::SetSceneEditorCameraData(Pg::Data::CameraData camData)
	{

	}

	void* EditorAdapter::GetEditorCameraViewSRV()
	{
		return _processMain->GetEngineGraphicsAdapter()->GetEditorCameraViewSRV();
	}

	void* EditorAdapter::GetGameCameraViewSRV()
	{
		return _processMain->GetEngineGraphicsAdapter()->GetGameCameraViewSRV();
	}

	void EditorAdapter::SetEditorMode(Pg::Data::Enums::eEditorMode mode)
	{
		_mode = mode;
		_processMain->GetEngineGraphicsAdapter()->SetEditorMode(mode);
	}

	Pg::Data::Enums::eEditorMode EditorAdapter::GetEditorMode()
	{
		return _mode;
	}

	void EditorAdapter::SetSceneList(std::vector<Pg::Data::Scene*> scenes)
	{
		if (_scenes == scenes) return;

		_scenes = scenes;
		_processMain->GetEngineGraphicsAdapter()->SetSceneList(_scenes);
	}

	void EditorAdapter::SetCurrentScene(Pg::Data::Scene* scene)
	{
		/*if (_currentScene == scene || scene->GetSceneName() == _currentScene->GetSceneName())
		{
			return;
		}*/

		if (_currentScene != nullptr)
		{
			if (scene->GetSceneName() == _currentScene->GetSceneName())
			{
				return;
			}
		}

		_currentScene = scene;
		_processMain->GetEngineGraphicsAdapter()->SetCurrentScene(_currentScene);
	}

	Pg::Data::Scene* EditorAdapter::GetCurrentScene()
	{
		return _processMain->GetEngineGraphicsAdapter()->GetCurrentScene();
	}

	void EditorAdapter::SetAddObjectList(const std::vector<Pg::Data::GameObject*>* addObjList)
	{
		_processMain->GetEngineGraphicsAdapter()->AddRenderObject_Runtime(addObjList);
	}

	void EditorAdapter::SetModifyObjectList(const std::vector<Pg::Data::GameObject*>* modifyObjList)
	{
		_processMain->GetEngineGraphicsAdapter()->ModifyRenderObject_Runtime(modifyObjList);
	}

	void EditorAdapter::SetDeleteObjectList(const std::vector<Pg::Data::GameObject*>* deleteObjList)
	{
		_processMain->GetEngineGraphicsAdapter()->DeleteRenderObject_Runtime(deleteObjList);
	}
}