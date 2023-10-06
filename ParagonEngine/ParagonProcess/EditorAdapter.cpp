#include "EditorAdapter.h"

namespace Pg::Core
{
	EditorAdapter::EditorAdapter(CoreMain* core) : _coreMain(core), _editorMode{}
	{

	}

	EditorAdapter::~EditorAdapter()
	{

	}

	void EditorAdapter::SetSceneEditorCameraData(Pg::Data::CameraData camData)
	{

	}

	void* EditorAdapter::GetSceneEditorViewport()
	{
		return {};
	}

	void* EditorAdapter::GetGameViewport()
	{
		return {};
	}

	void EditorAdapter::SetEditorMode(Pg::Data::Enums::eEditorMode mode)
	{
		_editorMode = mode;
	}

	void EditorAdapter::LoadEditorResource(Pg::Data::Enums::eAssetDefine define, void* memory, int sizeInBytes)
	{
		//
	}

}