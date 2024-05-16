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
		_processMain->GetEngineGraphicsAdapter()->SetSceneList(scenes);
	}

	std::vector<Pg::Data::Scene*> EditorAdapter::GetSceneList()
	{
		return  _processMain->GetEngineGraphicsAdapter()->GetSceneList();
	}

	void EditorAdapter::SetCurrentScene(Pg::Data::Scene* scene)
	{
		_processMain->GetEngineGraphicsAdapter()->SetCurrentScene(scene);
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