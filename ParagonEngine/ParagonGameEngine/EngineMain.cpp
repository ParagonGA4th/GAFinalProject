#include "EngineMain.h"

#include "PhysicSystem.h"
#include "SceneSystem.h"
#include "DebugSystem.h"
#include "SoundSystem.h"
#include "TweenSystem.h"
#include "Navigation.h"
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
#include "../ParagonMath/PgMath.h"
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
		auto& tNavSystem = singleton<Navigation>();
		_navSystem = &tNavSystem;

		//DeltaTime을 받기 위해 외부적으로 Util의 싱글턴을 갖고 와서 활용.
		//더 이상 엔진에서 제어권은 없다.
		auto& tTimeSystem = singleton<Pg::Util::Time::TimeSystem>();
		_timeSystem = &tTimeSystem;

		//트윈 자체는 Util에 머무름, TweenSystem은 Engine에.
		auto& tTweenSystem = singleton<Pg::Engine::TweenSystem>();
		_tweenSystem = &tTweenSystem;
	}

	EngineMain::~EngineMain()
	{

	}

	void EngineMain::Initialize(void* hwnd, float width, float height, const std::string& resourceListPath)
	{
		_sceneSystem->Initialize();
		_debugSystem->Initialize();
		_tweenSystem->Initialize();
		_physicSystem->Initialize(_debugSystem);
		_soundSystem->Initialize(resourceListPath);

		///Recast 관련 테스트.
		///맵 경로의 수정으로 Recast를 사용할 일이 없어져서 지움.
		//_navSystem->Initialize();
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/BattleSpace_res2.obj", 0);
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/TestingRecast_DoubleScale_FlippedInBlender.obj", 0);
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/TestingRecast_DoubleScale_FlippedInBlender.obj", 1);
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/TestingRecast_DoubleScale_FlippedInBlender.obj", 2);
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/TestingRecast_DoubleScale_FlippedInBlender.obj", 3);
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/TestingRecast_DoubleScale_FlippedInBlender.obj", 4);
		_behaviorTreeSystem->Initialize(resourceListPath);

		///Recast관련 테스트 코드.
		//_navTestInfo = new Pg::Data::NavMeshInfo;
		//_navTestInfo->vertices = new std::vector<Pg::Math::PGFLOAT3>();
		//_navTestInfo->indices = new std::vector<unsigned int>();
		//_navTestInfo->path = "TestForDifference";
		//_navSystem->GetNavmeshRenderInfo(0, *(_navTestInfo->vertices), *(_navTestInfo->indices));
		//_debugSystem->DrawNavMeshDebug(_navTestInfo); //한번만 추가해줬다. 클리어하지 않음.
		//_navSystem->SetAgent(2, 2.f, 45.f, 2.f, 0.9f);
		//_navSystem->SetSEpos(2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f);
	}

	void EngineMain::Update()
	{	 
		if (_currentRecordedEditMode != _previousEditMode)
		{
			if (_currentRecordedEditMode == Data::Enums::eEditorMode::_NONE ||
				_currentRecordedEditMode == Data::Enums::eEditorMode::_EDIT)
			{
				_sceneSystem->GetCurrentScene()->SetMainCamera(_sceneSystem->GetCurrentScene()->GetEditorCamera());
				
				//Editor Camera 오브젝트 살리기.

				_sceneSystem->GetCurrentScene()->GetEditorCamera()->SetActive(true);
				_sceneSystem->GetCurrentScene()->GetEditorCamera()->_object->SetActive(true);
			}
			else
			{
				//Editor Camera 오브젝트 작동 정지.
				PG_WARN("에디터 카메라가 저 멀리 가 있더라도, SetMainCamera 스크립트 내부 호출 안했을 때 Unity SceneView 처럼 볼 수 있어야. 불편 감수.");
				//_sceneSystem->GetCurrentScene()->GetEditorCamera()->SetActive(false);
				//_sceneSystem->GetCurrentScene()->GetEditorCamera()->_object->SetActive(false);

				//Scene에서 다시 InternalAwake / Awake / Start가 호출될 수 있게.
				std::for_each(_sceneSystem->GetCurrentScene()->GetObjectList().begin(),
					_sceneSystem->GetCurrentScene()->GetObjectList().end(), [](auto& iter)
					{ iter->ResetDebouncerBoolean(); });

				_sceneSystem->OnStopScene();

				//막 Play된 것이면 Start가 막 되는 것.
				_sceneSystem->_isStarted = false;
				//리셋, 클라이언트 딴에서 SetMainCamera 명시적으로 해줘야 하게. -> 이거 호환 위해 nullptr set은 꺼놨지만, 인게임에서 오버라이드 되어야 함.
				//_sceneSystem->GetCurrentScene()->SetMainCamera(nullptr);
			}
		}

		//다른 로직 외적으로, 보관용이고 로직 상으로 아무것도 안하는 Editor Mode 정보만 저장.
		_sceneSystem->PassEditorModeForStoring(_currentRecordedEditMode);

		//외적인 SceneLoad 로직 Start 관련, 항상 SceneSystem 체크한다.
		_sceneSystem->DebounceSceneLoadStatus(_currentRecordedEditMode);
		
		//PG_TRACE(_sceneSystem->GetCurrentScene()->GetSceneName());

		//기록된 Edit Mode가 EDIT/NONE으로 설정되었을시, 실행을 Update를 실행하지 않는다.
		//Early Return.
		if (_currentRecordedEditMode == Data::Enums::eEditorMode::_NONE ||
			_currentRecordedEditMode == Data::Enums::eEditorMode::_EDIT)
		{
			//Internal 함수들만 호출.
			bool tWorking = _sceneSystem->Update(false);
		}
		else
		{
			//Internal 함수들 + 게임 내부 로직 업데이트 함수 활용.
			_sceneSystem->BeforePhysicsUpdateInGame(); //Physics 발동 전 업데이트가 필요하다면! 심지어 Awake()보다도 전이다. 이제 BeforePhysicsAwake()까지 포함.
			_physicSystem->UpdatePhysics(_timeSystem->GetDeltaTime());
			_physicSystem->Flush();

			//SceneSystem은 동일 프레임에 씬이 바뀌었는지를 반환한다.
			bool tIsJustChangedScene = _sceneSystem->Update(true);

			_tweenSystem->Update();
			_soundSystem->Update();

			//만약 동일 프레임에 씬이 바뀌지 않았을 때만, BehaviorTreeSystem을 업데이트한다.
			if (!tIsJustChangedScene)
			{
				_behaviorTreeSystem->Update();
			}

			_physicSystem->UpdateTransform();
			_physicSystem->ApplyRuntimeChangesCollider(); // 현재로서는 하는 거 없음. 
		}

		///Recast관련 업데이트
		//_navSystem->HandleUpdate(_timeSystem->GetDeltaTime());

		
		///Mesh가 빌드 된 후 그려야 하기에, 더 나중에 있어야 한다.
		_debugSystem->EnableToggleDebugOnOff();
		_debugSystem->Update(_sceneSystem->GetCurrentScene());



		///Recast Obj 파일 디버그 그리기
		//std::vector<std::pair<Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT3>> navipos1 = _navSystem->FindStraightPath(0);
		//std::vector<std::pair<Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT3>> navipos2 = _navSystem->FindStraightPath(2);
		//Pg::Math::PGFLOAT3 navipos3 = _navSystem->FindRaycastPath(0);

		//for (auto path : navipos1)
		//{
		//	_debugSystem->DrawLineDebug(path.first, path.second, Pg::Math::PGFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		//}
		
		//for (auto path : navipos2)
		//{
		//	_debugSystem->DrawLineDebug(path.first, path.second, Pg::Math::PGFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
		//}

		//_debugSystem->DrawLineDebug(Pg::Math::PGFLOAT3(0.0f, 0.0f, 0.0f), navipos3, Pg::Math::PGFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));

		//명시적으로 바뀔 때 감지를 할 수 있게 하기 위해, 
		//현재의 Editor Mode를 전의 것이라고 대입한다.
		//EditorMode가 매프레임마다 호출되는 것이 아니기 때문.
		_previousEditMode = _currentRecordedEditMode;
	}

	void EngineMain::Finalize()
	{
		_physicSystem->Finalize();
		//_navSystem->Finalize();
		_tweenSystem->Finalize();
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

	std::vector<Pg::Data::Scene*> EngineMain::GetSceneList()
	{
		return _sceneSystem->GetSceneList();
	}

	void EngineMain::SetCurrentScene(Pg::Data::Scene* currentScene)
	{
		// 임시
		if (currentScene != nullptr)
		{
			_sceneSystem->SetCurrentScene(currentScene->GetSceneName());
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

	const std::vector<Pg::Data::NavMeshInfo*>& EngineMain::GetNavMeshDebugData() const
	{
		return _debugSystem->GetNavMeshVector();
	}

	const std::vector<Pg::Data::NavCylinderInfo*>& EngineMain::GetNavCylinderDebugData() const
	{
		return _debugSystem->GetNavCylinderVector();
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
		
		//현재로서는 NavMesh를 지우지 않는다. 매 프레임마다 삭제가 아닌, Scene이 바뀔 때마다 로드될 것이기 때문.
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
		//이미 있던거 기록.
		_previousEditMode = _currentRecordedEditMode;

		//기존의 Editor Mode Enum 기록.
		_currentRecordedEditMode = editorMode;
	}

	void EngineMain::AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{
		_physicSystem->CheckAddRuntimeColliders(objVecP);
	}

	void EngineMain::ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{

	}

	void EngineMain::DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{
		_physicSystem->CheckDeleteRuntimeColliders(objVecP);
	}

	void EngineMain::OnParagonProjectLoad(const std::vector<Pg::Data::Scene*>& sceneVec)
	{
		_soundSystem->LoadSoundListOnProjectLoad(sceneVec);
		_sceneSystem->SetProjectSceneList_GrabManagedObjects(sceneVec);
	}

}
