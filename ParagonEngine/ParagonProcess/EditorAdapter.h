#pragma once
/// <summary>
/// Process에서 Editor를 조종할 수 있는 어댑터의 역할을 수행한다.
/// </summary>

#include "../ParagonData/CameraData.h"
#include "../ParagonData/AssetDefines.h"
#include "../ParagonData/EditorMode.h"

#include <vector>

namespace Pg::Core
{
	class ProcessMain;
}

namespace Pg::Data
{
	class Scene;
	class GameObject;
}

namespace Pg::Core
{
	class EditorAdapter
	{
	public:
		EditorAdapter(ProcessMain* core);
		~EditorAdapter();

		//에디터용 카메라 데이터를 설정한다.
		void SetSceneEditorCameraData(Pg::Data::CameraData camData);
		
		//곧, 에디터 연동에 필요한 Editor의 SRV를 리턴.
		void* GetEditorCameraViewSRV();

		//실제 게임 내 메인 카메라 리턴.
		void* GetGameCameraViewSRV();

		//에디터에 따라 바뀐 상태를 설정한다.
		void SetEditorMode(Pg::Data::Enums::eEditorMode mode);
		Pg::Data::Enums::eEditorMode GetEditorMode();

		//에디터에서 가져온 씬 목록을 설정한다
		void SetSceneList(std::vector<Pg::Data::Scene*> scenes);
		std::vector<Pg::Data::Scene*> GetSceneList();

		//에디터에 따라 현재 씬을 설정한다
		void SetCurrentScene(Pg::Data::Scene* scene);
		Pg::Data::Scene* GetCurrentScene();

		void SetAddObjectList(const std::vector<Pg::Data::GameObject*>* addObjList);
		void SetModifyObjectList(const std::vector<Pg::Data::GameObject*>* modifyObjList);
		void SetDeleteObjectList(const std::vector<Pg::Data::GameObject*>* deleteObjList);

	private:
		ProcessMain* _processMain;
		Pg::Data::Enums::eEditorMode _mode;
	};
}


