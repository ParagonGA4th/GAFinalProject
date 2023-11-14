#include "EngineMain.h"
#include "InputSystem.h"
#include "PhysicSystem.h"
#include "SceneSystem.h"
#include "TimeSystem.h"
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
	EngineMain::EngineMain(Pg::Core::ProcessMain* core) : _coreMain(core), _engineResourceManager(Manager::EngineResourceManager::Instance())
	{
		//Input
		auto& tInputSystem = singleton<Input::InputSystem>();
		_inputSystem = &tInputSystem;

		//Physic
		auto& tphysicSystem = singleton<Physic::PhysicSystem>();
		_physicSystem = &tphysicSystem;

		//Scene
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		//Time
		auto& tTimeSystem = singleton<Time::TimeSystem>();
		_timeSystem = &tTimeSystem;
	}

	EngineMain::~EngineMain()
	{

	}


	void EngineMain::Initialize(float width, float height)
	{
		_sceneSystem->Initialize();
		_inputSystem->Initialize(width, height);
		_physicSystem->Initialize();
		_timeSystem->Initialize();
	}

	void EngineMain::Update()
	{
		_sceneSystem->Update();
		_inputSystem->Update();
		_physicSystem->UpdatePhysics();
		_timeSystem->TimeMeasure();

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
	}

	void EngineMain::LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define)
	{

	}

	void EngineMain::UnloadResource(const std::string& filePath)
	{
		//Load와 달리, 동시에 두 개의 리소스 매니저가 동시에 호출된다. //지우지 못했어도 오류 반환하지 말자!
	}

	Pg::Data::Scene* EngineMain::GetCurrentScene()
	{
		return _sceneSystem->GetCurrentScene();
	}

	Pg::Data::CameraData* EngineMain::GetCameraData()
	{
		return _sceneSystem->GetCurrentScene()->GetMainCamera()->GetCameraData();
	}

}
