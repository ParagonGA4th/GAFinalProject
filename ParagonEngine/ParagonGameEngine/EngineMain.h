#pragma once

#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonProcess/IEngine.h"
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/DebugData.h"

#include <vector>
/// <summary>
/// 2023.09.08
/// </summary>

namespace Pg::Util
{
	namespace Time
	{
		class TimeSystem;
	}
}

namespace Pg::Core
{
	class ProcessMain;
}

namespace Pg::Data
{
	struct CameraData;
	struct BoxInfo;
	class Scene;
}

namespace Pg::Engine
{
	namespace Input
	{
		class InputSystem;
	}

	namespace Physic
	{
		class PhysicSystem;
	}

	namespace BTree
	{
		class BehaviorTreeSystem;
	}

	namespace Manager
	{
		class EngineResourceManager;
	}

	class SceneSystem;

	class DebugSystem;

	class SoundSystem;

	class Navigation;

	class TweenSystem;
}

namespace Pg::Engine
{
	class EngineMain : public Pg::Core::IEngine
	{
	public:
		PARAGON_ENGINE_DLL EngineMain(Pg::Core::ProcessMain* core);
		virtual ~EngineMain();

		PARAGON_ENGINE_DLL virtual void Initialize(void* hwnd, float width, float height, const std::string& resourceListPath) override;
		PARAGON_ENGINE_DLL virtual void SetEditorMode(Pg::Data::Enums::eEditorMode editorMode) override;
		PARAGON_ENGINE_DLL virtual void Update() override;
		PARAGON_ENGINE_DLL virtual void Finalize() override;

		//AssetManager와의 교류를 위해, 엔진 리소스 매니저에 리소스를 로드하는 함수.
		PARAGON_ENGINE_DLL virtual void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define) override;

		//AssetManager와의 교류를 위해, 엔진 리소스 매니저에 리소스를 언로드하는 함수.
		PARAGON_ENGINE_DLL virtual void UnloadResource(const std::string& filePath) override;

		PARAGON_ENGINE_DLL virtual void SetSceneList(std::vector<Pg::Data::Scene*> sceneList) override;
		PARAGON_ENGINE_DLL virtual std::vector<Pg::Data::Scene*> GetSceneList() override;
		PARAGON_ENGINE_DLL virtual void SetCurrentScene(Pg::Data::Scene* currentScene) override;
		PARAGON_ENGINE_DLL virtual Pg::Data::Scene* GetCurrentScene() override;

		PARAGON_ENGINE_DLL virtual Pg::Data::CameraData* GetCameraData() override;

		PARAGON_ENGINE_DLL virtual const std::vector<Pg::Data::BoxInfo*>& GetBoxDebugData() const override;

		PARAGON_ENGINE_DLL virtual const std::vector<Pg::Data::LineInfo>& GetLineDebugData() const override;
		PARAGON_ENGINE_DLL virtual const std::vector<Pg::Data::SphereInfo*>& GetSphereDebugData() const override;
		PARAGON_ENGINE_DLL virtual const std::vector<Pg::Data::CapsuleInfo*>& GetCapsuleDebugData() const override;
		PARAGON_ENGINE_DLL virtual const std::vector<Pg::Data::PlaneInfo*>& GetPlaneDebugData() const override;
		PARAGON_ENGINE_DLL virtual const std::vector<Pg::Data::NavMeshInfo*>& GetNavMeshDebugData() const override;
		PARAGON_ENGINE_DLL virtual const std::vector<Pg::Data::NavCylinderInfo*>& GetNavCylinderDebugData() const override;
		PARAGON_ENGINE_DLL virtual const std::vector<Pg::Data::RayCastInfo>& GetRayCastDebugData() const override;
		PARAGON_ENGINE_DLL virtual const std::vector<Pg::Data::Box2DInfo>& GetBox2dDebugData() const override;

		PARAGON_ENGINE_DLL virtual void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) override;
		PARAGON_ENGINE_DLL virtual void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) override;
		PARAGON_ENGINE_DLL virtual void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) override;

		PARAGON_ENGINE_DLL virtual void ClearDebugVectorData() override;

		PARAGON_ENGINE_DLL virtual float GetDeltaTime() override;

		//씬이 바뀌었는지 다른 파트들에게 알려줄 수 있다. nullptr이면 안 변했던 것.
		PARAGON_ENGINE_DLL virtual Pg::Data::Scene* NotifyIfChangedScene() override;
			
		PARAGON_ENGINE_DLL virtual void OnParagonProjectLoad(const std::vector<Pg::Data::Scene*>& sceneVec) override;
	private:
		Pg::Core::ProcessMain* _coreMain = nullptr;

		Physic::PhysicSystem* _physicSystem = nullptr;
		BTree::BehaviorTreeSystem* _behaviorTreeSystem = nullptr;
		SceneSystem* _sceneSystem = nullptr;
		DebugSystem* _debugSystem = nullptr;
		SoundSystem* _soundSystem = nullptr;
		TweenSystem* _tweenSystem = nullptr;
		Navigation* _navSystem = nullptr;
		Pg::Engine::Manager::EngineResourceManager* _engineResourceManager = nullptr;

		Pg::Util::Time::TimeSystem* _timeSystem = nullptr; //제어권은 더이상 엔진에는 없다.

		Pg::Data::Enums::eEditorMode _currentRecordedEditMode{ Data::Enums::eEditorMode::_NONE };
		Pg::Data::Enums::eEditorMode _previousEditMode{ Data::Enums::eEditorMode::_NONE };

	private:
		Pg::Data::NavMeshInfo* _navTestInfo{ nullptr };
		Pg::Data::NavCylinderInfo* _navCyilnderInfo{ nullptr };
	};
}

