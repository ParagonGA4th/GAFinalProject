#include "EngineMain.h"

#include "PhysicSystem.h"
#include "SceneSystem.h"
#include "DebugSystem.h"
#include "SoundSystem.h"
#include "TweenSystem.h"
#include "Navigation.h"
#include "BehaviorTreeSystem.h"
#include "EngineResourceManager.h"


//��� �ܺ� SDK ���� �� ���� ����
#include "PhysicSystem.h"

//<���� GameEngine Resource�� ���>

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

		//DeltaTime�� �ޱ� ���� �ܺ������� Util�� �̱����� ���� �ͼ� Ȱ��.
		//�� �̻� �������� ������� ����.
		auto& tTimeSystem = singleton<Pg::Util::Time::TimeSystem>();
		_timeSystem = &tTimeSystem;

		//Ʈ�� ��ü�� Util�� �ӹ���, TweenSystem�� Engine��.
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

		///Recast ���� �׽�Ʈ.
		///�� ����� �������� Recast�� ����� ���� �������� ����.
		//_navSystem->Initialize();
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/BattleSpace_res2.obj", 0);
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/TestingRecast_DoubleScale_FlippedInBlender.obj", 0);
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/TestingRecast_DoubleScale_FlippedInBlender.obj", 1);
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/TestingRecast_DoubleScale_FlippedInBlender.obj", 2);
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/TestingRecast_DoubleScale_FlippedInBlender.obj", 3);
		//_navSystem->HandleBuild("../Resources/3DModels/StaticMesh/TestingRecast/TestingRecast_DoubleScale_FlippedInBlender.obj", 4);
		_behaviorTreeSystem->Initialize(resourceListPath);

		///Recast���� �׽�Ʈ �ڵ�.
		//_navTestInfo = new Pg::Data::NavMeshInfo;
		//_navTestInfo->vertices = new std::vector<Pg::Math::PGFLOAT3>();
		//_navTestInfo->indices = new std::vector<unsigned int>();
		//_navTestInfo->path = "TestForDifference";
		//_navSystem->GetNavmeshRenderInfo(0, *(_navTestInfo->vertices), *(_navTestInfo->indices));
		//_debugSystem->DrawNavMeshDebug(_navTestInfo); //�ѹ��� �߰������. Ŭ�������� ����.
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
				
				//Editor Camera ������Ʈ �츮��.

				_sceneSystem->GetCurrentScene()->GetEditorCamera()->SetActive(true);
				_sceneSystem->GetCurrentScene()->GetEditorCamera()->_object->SetActive(true);
			}
			else
			{
				//Editor Camera ������Ʈ �۵� ����.
				PG_WARN("������ ī�޶� �� �ָ� �� �ִ���, SetMainCamera ��ũ��Ʈ ���� ȣ�� ������ �� Unity SceneView ó�� �� �� �־��. ���� ����.");
				//_sceneSystem->GetCurrentScene()->GetEditorCamera()->SetActive(false);
				//_sceneSystem->GetCurrentScene()->GetEditorCamera()->_object->SetActive(false);

				//Scene���� �ٽ� InternalAwake / Awake / Start�� ȣ��� �� �ְ�.
				std::for_each(_sceneSystem->GetCurrentScene()->GetObjectList().begin(),
					_sceneSystem->GetCurrentScene()->GetObjectList().end(), [](auto& iter)
					{ iter->ResetDebouncerBoolean(); });

				_sceneSystem->OnStopScene();

				//�� Play�� ���̸� Start�� �� �Ǵ� ��.
				_sceneSystem->_isStarted = false;
				//����, Ŭ���̾�Ʈ ������ SetMainCamera ��������� ����� �ϰ�. -> �̰� ȣȯ ���� nullptr set�� ��������, �ΰ��ӿ��� �������̵� �Ǿ�� ��.
				//_sceneSystem->GetCurrentScene()->SetMainCamera(nullptr);
			}
		}

		//�ٸ� ���� ��������, �������̰� ���� ������ �ƹ��͵� ���ϴ� Editor Mode ������ ����.
		_sceneSystem->PassEditorModeForStoring(_currentRecordedEditMode);

		//������ SceneLoad ���� Start ����, �׻� SceneSystem üũ�Ѵ�.
		_sceneSystem->DebounceSceneLoadStatus(_currentRecordedEditMode);
		
		//PG_TRACE(_sceneSystem->GetCurrentScene()->GetSceneName());

		//��ϵ� Edit Mode�� EDIT/NONE���� �����Ǿ�����, ������ Update�� �������� �ʴ´�.
		//Early Return.
		if (_currentRecordedEditMode == Data::Enums::eEditorMode::_NONE ||
			_currentRecordedEditMode == Data::Enums::eEditorMode::_EDIT)
		{
			//Internal �Լ��鸸 ȣ��.
			bool tWorking = _sceneSystem->Update(false);
		}
		else
		{
			//Internal �Լ��� + ���� ���� ���� ������Ʈ �Լ� Ȱ��.
			_sceneSystem->BeforePhysicsUpdateInGame(); //Physics �ߵ� �� ������Ʈ�� �ʿ��ϴٸ�! ������ Awake()���ٵ� ���̴�. ���� BeforePhysicsAwake()���� ����.
			_physicSystem->UpdatePhysics(_timeSystem->GetDeltaTime());
			_physicSystem->Flush();

			//SceneSystem�� ���� �����ӿ� ���� �ٲ�������� ��ȯ�Ѵ�.
			bool tIsJustChangedScene = _sceneSystem->Update(true);

			_tweenSystem->Update();
			_soundSystem->Update();

			//���� ���� �����ӿ� ���� �ٲ��� �ʾ��� ����, BehaviorTreeSystem�� ������Ʈ�Ѵ�.
			if (!tIsJustChangedScene)
			{
				_behaviorTreeSystem->Update();
			}

			_physicSystem->UpdateTransform();
			_physicSystem->ApplyRuntimeChangesCollider(); // ����μ��� �ϴ� �� ����. 
		}

		///Recast���� ������Ʈ
		//_navSystem->HandleUpdate(_timeSystem->GetDeltaTime());

		
		///Mesh�� ���� �� �� �׷��� �ϱ⿡, �� ���߿� �־�� �Ѵ�.
		_debugSystem->EnableToggleDebugOnOff();
		_debugSystem->Update(_sceneSystem->GetCurrentScene());



		///Recast Obj ���� ����� �׸���
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

		//��������� �ٲ� �� ������ �� �� �ְ� �ϱ� ����, 
		//������ Editor Mode�� ���� ���̶�� �����Ѵ�.
		//EditorMode�� �������Ӹ��� ȣ��Ǵ� ���� �ƴϱ� ����.
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
		//Load�� �޸�, ���ÿ� �� ���� ���ҽ� �Ŵ����� ���ÿ� ȣ��ȴ�. //������ ���߾ ���� ��ȯ���� ����!
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
		// �ӽ�
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
		//�ϴ��� �ڽ��� �ٷ��.
		_debugSystem->DeleteBoxDebug();
		_debugSystem->DeleteSphereDebug();
		_debugSystem->DeleteCapsuleDebug();
		_debugSystem->DeleteLineDebug();
		_debugSystem->DeletePlaneDebug();
		_debugSystem->DeleteRayCastDebug();
		_debugSystem->DeleteBox2DDebug();
		
		//����μ��� NavMesh�� ������ �ʴ´�. �� �����Ӹ��� ������ �ƴ�, Scene�� �ٲ� ������ �ε�� ���̱� ����.
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
		//�̹� �ִ��� ���.
		_previousEditMode = _currentRecordedEditMode;

		//������ Editor Mode Enum ���.
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
