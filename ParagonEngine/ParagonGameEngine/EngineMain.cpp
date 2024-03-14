#include "EngineMain.h"

#include "PhysicSystem.h"
#include "SceneSystem.h"
#include "DebugSystem.h"
#include "TimeSystem.h"
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

		//Time
		auto& tTimeSystem = singleton<Time::TimeSystem>();
		_timeSystem = &tTimeSystem;

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
		_timeSystem->Initialize(hwnd);
	}

	void EngineMain::Update()
	{
		///업데이트 순서 무조건 고정!!! 바뀌면 안됨
		_timeSystem->TimeMeasure();
		
		_physicSystem->UpdatePhysics(_timeSystem->GetDeltaTime());
		_physicSystem->Flush();
		_sceneSystem->Update();
		_soundSystem->Update();
		_navSystem->Update(_timeSystem->GetDeltaTime());
		_behaviorTreeSystem->Update();
		_physicSystem->UpdateTransform();
		_debugSystem->Update(_sceneSystem->GetCurrentScene());
		

		 static bool tTest = false;
		if (!tTest)
		{
			PG_TRACE("Debugger Used In ParagonGameEngine!");
			tTest = true;
		}

		/*if(_inputSystem->GetKey(API::Input::MouseLeft))
		{
			PG_TRACE("INPUTSYSTEM WORKS!");
		}*/
		
	}

	void EngineMain::Finalize()
	{
		_physicSystem->Finalize();
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
		//return _sceneSystem->GetCurrentScene();
	}

	void EngineMain::SetCurrentScene(Pg::Data::Scene* currentScene)
	{
		// 임시
		if (currentScene != nullptr)
		{
			_sceneSystem->SetCurrentScene(currentScene);
			_sceneSystem->_isStarted = false;
		}

		//_sceneSystem->SetSceneData(currentScene);

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

	
}
