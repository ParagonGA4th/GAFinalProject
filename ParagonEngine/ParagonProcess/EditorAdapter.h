#pragma once
/// <summary>
/// Process���� Editor�� ������ �� �ִ� ������� ������ �����Ѵ�.
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

		//�����Ϳ� ī�޶� �����͸� �����Ѵ�.
		void SetSceneEditorCameraData(Pg::Data::CameraData camData);
		
		//��, ������ ������ �ʿ��� Editor�� SRV�� ����.
		void* GetEditorCameraViewSRV();

		//���� ���� �� ���� ī�޶� ����.
		void* GetGameCameraViewSRV();

		//�����Ϳ� ���� �ٲ� ���¸� �����Ѵ�.
		void SetEditorMode(Pg::Data::Enums::eEditorMode mode);
		Pg::Data::Enums::eEditorMode GetEditorMode();

		//�����Ϳ��� ������ �� ����� �����Ѵ�
		void SetSceneList(std::vector<Pg::Data::Scene*> scenes);
		std::vector<Pg::Data::Scene*> GetSceneList();

		//�����Ϳ� ���� ���� ���� �����Ѵ�
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


