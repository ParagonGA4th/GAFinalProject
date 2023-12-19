#include "EditorAdapter.h"
#include "ProcessMain.h"
#include "EngineGraphicsAdapter.h"

namespace Pg::Core
{
	EditorAdapter::EditorAdapter(ProcessMain* core) : _processMain(core), _editorMode{}
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
		_editorMode = mode;
	}

	void EditorAdapter::LoadEditorResource(Pg::Data::Enums::eAssetDefine define, void* memory, int sizeInBytes)
	{
		//
	}

	void EditorAdapter::SetSceneList(std::vector<Pg::Data::Scene*> scenes)
	{
		_scenes = scenes;
	}

	std::vector<Pg::Data::Scene*> EditorAdapter::GetSceneList()
	{
		return _scenes;
	}

	void EditorAdapter::SetCurrentScene(Pg::Data::Scene* scene)
	{
		_currentScene = scene;
	}

	Pg::Data::Scene* EditorAdapter::GetCurrentScene()
	{
		return _currentScene;
	}

}