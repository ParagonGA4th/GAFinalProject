#include "EngineMain.h"

#include "PhysicSystem.h"
#include "SceneSystem.h"
#include "DebugSystem.h"
#include "SoundSystem.h"
#include "NavigationSystem.h"
#include "BehaviorTreeSystem.h"
#include "EngineResourceManager.h"


//얘는 외부 SDK 연동 후 진행 예정
#include "PhysicSystem.h"

//<실제 GameEngine Resource의 목록>

//</>

#include "../ParagonProcess/ProcessMain.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/Camera.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonAPI/KeyCodeType.h"
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonMath.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonMath.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonData.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonData.lib")
#endif // _DEBUG

namespace Pg::Engine
{
	EngineMain::EngineMain(Pg::Core::ProcessMain* core) : 
		_coreMain(core), 
		_engineResourceManager(Manager::EngineResourceManager::Instance())
	{
		//Physic
		auto& tphysicSystem = singleton<Physic::PhysicSystem>();
		_physicSystem = &tphysicSystem;

		//Scene
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		//Debug
		auto& tDebugSystem = singleton<DebugSystem>();
		_debugSystem = &tDebugSystem;

		//Sound
		auto& tSoundSystem = singleton<SoundSystem>();
		_soundSystem = &tSoundSystem;

		//BehaviorTree
		auto& tBTreeSystem = singleton<BTree::BehaviorTreeSystem>();
		_behaviorTreeSystem = &tBTreeSystem;

		//Navigation
		auto& tNavSystem = singleton<NavigationSystem>();
		_navSystem = &tNavSystem;

		//DeltaTime을 받기 위해 외부적으로 Util의 싱글턴을 갖고 와서 활용.
		//더 이상 엔진에서 제어권은 없다.
		auto& tTimeSystem = singleton<Pg::Util::Time::TimeSystem>();
		_timeSystem = &tTimeSystem;
	}

	EngineMain::~EngineMain()
	{

	}

	void EngineMain::Initialize(void* hwnd, float width, float height, const std::string& resourceListPath)
	{
		_sceneSystem->Initialize();
		_debugSystem->Initialize();
		_physicSystem->Initialize(_debugSystem);
		_soundSystem->Initialize(resourceListPath);
		_navSystem->Initialize();
		_behaviorTreeSystem->Initialize(resourceListPath);
	}

	void EngineMain::Update()
	{
		//외적인 SceneLoad 로직 Start 관련, 항상 SceneSystem 체크한다.
		_sceneSystem->DebounceSceneLoadStatus();

		//기록된 Edit Mode가 EDIT/NONE으로 설정되었을시, 실행을 Update를 실행하지 않는다.
		//Early Return.
		if (_prevRecordedEditMode == Data::Enums::eEditorMode::_NONE ||
			_prevRecordedEditMode == Data::Enums::eEditorMode::_EDIT)
		{
			//Internal 함수들만 호출.
			_sceneSystem->Update(false);
		}
		else
		{
			//Internal 함수들 + 게임 내부 로직 업데이트 함수 활용.
			_physicSystem->UpdatePhysics(_timeSystem->GetDeltaTime());
			_physicSystem->Flush();
			_sceneSystem->Update(true);
			_soundSystem->Update();
			_navSystem->Update(_timeSystem->GetDeltaTime());
			_behaviorTreeSystem->Update();
			_physicSystem->UpdateTransform();
			_debugSystem->Update(_sceneSystem->GetCurrentScene());
		}
	}

	void EngineMain::Finalize()
	{
		_physicSystem->Finalize();
		_navSystem->Finalize();
		_soundSystem->Finalize();
	}

	void EngineMain::LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define)
	{

	}

	void EngineMain::UnloadResource(const std::string& filePath)
	{
		//Load와 달리, 동시에 두 개의 리소스 매니저가 동시에 호출된다. //지우지 못했어도 오류 반환하지 말자!
	}

	void EngineMain::SetSceneList(std::vector<Pg::Data::Scene*> sceneList)
	{
		_sceneSystem->SetSceneList(sceneList);
	}

	void EngineMain::SetCurrentScene(Pg::Data::Scene* currentScene)
	{
		// 임시
		if (currentScene != nullptr)
		{
			_sceneSystem->SetCurrentScene(currentScene);
		}
	}

	Pg::Data::Scene* EngineMain::GetCurrentScene()
	{
		return _sceneSystem->GetCurrentScene();
	}

	Pg::Data::CameraData* EngineMain::GetCameraData()
	{
		return _sceneSystem->GetCurrentScene()->GetMainCamera()->GetCameraData();
	}

	const std::vector<BoxInfo*>& EngineMain::GetBoxDebugData() const
	{
		return _debugSystem->GetBoxVector();
	}

	const std::vector<Pg::Data::LineInfo>& EngineMain::GetLineDebugData() const
	{
		return _debugSystem->GetLineVector();
	}

	const std::vector<Pg::Data::SphereInfo*>& EngineMain::GetSphereDebugData() const
	{
		return _debugSystem->GetSphereVector();
	}

	const std::vector<Pg::Data::CapsuleInfo*>& EngineMain::GetCapsuleDebugData() const
	{
		return _debugSystem->GetCapsuleVector();
	}

	const std::vector<Pg::Data::PlaneInfo*>& EngineMain::GetPlaneDebugData() const
	{
		return _debugSystem->GetPlaneVector();
	}

	const std::vector<Pg::Data::RayCastInfo>& EngineMain::GetRayCastDebugData() const
	{
		return _debugSystem->GetRayCastVector();
	}

	const std::vector<Pg::Data::Box2DInfo>& EngineMain::GetBox2dDebugData() const
	{
		return _debugSystem->GetBox2DVector();
	}

	void EngineMain::ClearDebugVectorData()
	{
		//일단은 박스만 다루니.
		_debugSystem->DeleteBoxDebug();
		_debugSystem->DeleteSphereDebug();
		_debugSystem->DeleteCapsuleDebug();
		_debugSystem->DeleteLineDebug();
		_debugSystem->DeletePlaneDebug();
		_debugSystem->DeleteRayCastDebug();
		_debugSystem->DeleteBox2DDebug();
	}

	float EngineMain::GetDeltaTime()
	{
		return _timeSystem->GetDeltaTime();
	}

	Pg::Data::Scene* EngineMain::NotifyIfChangedScene()
	{
		if (!_sceneSystem->GetIsStartedScene())
		{
			return _sceneSystem->GetCurrentScene();
		}
		else
		{
			return nullptr;
		}
	}

	void EngineMain::SetEditorMode(Pg::Data::Enums::eEditorMode editorMode)
	{
		//기존의 Editor Mode Enum 기록.
		_prevRecordedEditMode = editorMode;
	}

}
