#pragma once
#include "../ParagonData/AssetDefines.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/DebugData.h"
#include "../ParagonData/EditorMode.h"
#include <string>
#include <vector>

/// <summary>
/// 변지상의 엔진 인터페이스. 코어에 들어갈 예정
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

		//그래픽스 엔진에서 필요한 정보
		//virtual Scene* GetCurrentScene();

		//AssetManager와의 교류를 위해, 엔진 리소스 매니저에 리소스를 로드하는 함수.
		virtual void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define) abstract;

		//AssetManager와의 교류를 위해, 엔진 리소스 매니저에 리소스를 언로드하는 함수.
		virtual void UnloadResource(const std::string& filePath) abstract;

		//렌더링을 위해 현재의 씬을 받아올 필요가 있다.
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

		//디버그 데이터 디버그 시스템에서 클리어.
		virtual void ClearDebugVectorData() abstract;

		//델타타임 반환.
		virtual float GetDeltaTime() abstract;

		//씬이 바뀌었다고 다른 파트들에게 알려줄 수 있다. 안 바뀌었을시 Nullptr.
		virtual Pg::Data::Scene* NotifyIfChangedScene() abstract;

		//SceneManager한테 Project의 Scene을 알려주기 위한 용도이다.
		virtual void OnParagonProjectLoad(const std::vector<Pg::Data::Scene*>& sceneVec) abstract;
	};
}