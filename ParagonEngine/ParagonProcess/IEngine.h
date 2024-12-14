#pragma once
#include "../ParagonData/AssetDefines.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/DebugData.h"
#include "../ParagonData/EditorMode.h"
#include <string>
#include <vector>

/// <summary>
/// �������� ���� �������̽�. �ھ �� ����
/// 2023.09.11
/// </summary>

namespace Pg::Core
{
	class IEngine abstract
	{
	public:
		virtual void Initialize(void* hwnd, float width, float height, const std::string& resourceListPath) abstract;

		virtual void SetEditorMode(Pg::Data::Enums::eEditorMode editorMode) abstract;

		virtual void Update() abstract;

		virtual void Finalize() abstract;

		//�׷��Ƚ� �������� �ʿ��� ����
		//virtual Scene* GetCurrentScene();

		//AssetManager���� ������ ����, ���� ���ҽ� �Ŵ����� ���ҽ��� �ε��ϴ� �Լ�.
		virtual void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define) abstract;

		//AssetManager���� ������ ����, ���� ���ҽ� �Ŵ����� ���ҽ��� ��ε��ϴ� �Լ�.
		virtual void UnloadResource(const std::string& filePath) abstract;

		//�������� ���� ������ ���� �޾ƿ� �ʿ䰡 �ִ�.
		virtual void SetSceneList(std::vector<Pg::Data::Scene*> sceneList) abstract;
		virtual std::vector<Pg::Data::Scene*> GetSceneList() abstract;
		virtual void SetCurrentScene(Pg::Data::Scene* currentScene) abstract;
		virtual Pg::Data::Scene* GetCurrentScene() abstract;

		virtual Pg::Data::CameraData* GetCameraData() abstract;

		virtual const std::vector<Pg::Data::BoxInfo*>& GetBoxDebugData() const abstract;

		virtual const std::vector<Pg::Data::LineInfo>& GetLineDebugData() const abstract;
		virtual const std::vector<Pg::Data::SphereInfo*>& GetSphereDebugData() const abstract;
		virtual const std::vector<Pg::Data::CapsuleInfo*>& GetCapsuleDebugData() const abstract;
		virtual const std::vector<Pg::Data::PlaneInfo*>& GetPlaneDebugData() const abstract;
		virtual const std::vector<Pg::Data::NavCylinderInfo*>& GetNavCylinderDebugData() const abstract;
		virtual const std::vector<Pg::Data::NavMeshInfo*>& GetNavMeshDebugData() const abstract;
		virtual const std::vector<Pg::Data::RayCastInfo>& GetRayCastDebugData() const abstract;
		virtual const std::vector<Pg::Data::Box2DInfo>& GetBox2dDebugData() const abstract;
		
		virtual void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) abstract;
		virtual void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) abstract;
		virtual void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) abstract;

		//����� ������ ����� �ý��ۿ��� Ŭ����.
		virtual void ClearDebugVectorData() abstract;

		//��ŸŸ�� ��ȯ.
		virtual float GetDeltaTime() abstract;

		//���� �ٲ���ٰ� �ٸ� ��Ʈ�鿡�� �˷��� �� �ִ�. �� �ٲ������ Nullptr.
		virtual Pg::Data::Scene* NotifyIfChangedScene() abstract;

		//SceneManager���� Project�� Scene�� �˷��ֱ� ���� �뵵�̴�.
		virtual void OnParagonProjectLoad(const std::vector<Pg::Data::Scene*>& sceneVec) abstract;
	};
}