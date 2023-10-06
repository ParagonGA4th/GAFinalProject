#pragma once
/// <summary>
/// Process에서 Editor를 조종할 수 있는 어댑터의 역할을 수행한다.
/// </summary>

#include "../ParagonData/CameraData.h"
#include "../ParagonData/AssetDefines.h"
#include "../ParagonData/EditorMode.h"

namespace Pg::Core
{
	class CoreMain;
}

namespace Pg::Core
{
	class EditorAdapter
	{
	public:
		EditorAdapter(CoreMain* core);
		~EditorAdapter();

		//에디터용 카메라 데이터를 설정한다.
		void SetSceneEditorCameraData(Pg::Data::CameraData camData);
		
		//곧, 에디터 연동에 필요한 Scene Editor의 뷰포트를 리턴.
		void* GetSceneEditorViewport();

		//실제 게임 내 메인 카메라 리턴.
		void* GetGameViewport();

		//에디터에 따라 바뀐 상태를 설정한다.
		void SetEditorMode(Pg::Data::Enums::eEditorMode mode);

		//에디터에 고유한 리소스를 로드한다. 
		void LoadEditorResource(Pg::Data::Enums::eAssetDefine define, void* memory, int sizeInBytes);

	private:
		CoreMain* _coreMain;
		Pg::Data::Enums::eEditorMode _editorMode;
	};
}


