#include "TotalGameManager.h"

#include "../ParagonData/Scene.h"

#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTween.h"

#include <algorithm>
#include <cassert>
#include <singleton-cpp/singleton.h>

//Handler �ø���.
#include "BaseAreaHandler.h"
#include "BaseEnemyHandler.h"
#include "BaseGUIHandler.h"
#include "PlayerHandler.h"

//�ٸ� �Ŵ�����, etc.
#include "InGameManager.h"
#include "CombatSystem.h"

namespace Pg::DataScript
{
	//�̰� ���� ���� GrabManagedObject�� ȣ��Ǿ�� �Ѵ�.
	void TotalGameManager::Awake()
	{
		{
			static bool tAssureNoDoubleCall = true;
			assert(tAssureNoDoubleCall && "�ѹ��� �� ���� �÷ο� ���ο��� ȣ��Ǿ��");
			tAssureNoDoubleCall = false;
		}
	}

	void TotalGameManager::Start()
	{

	}

	void TotalGameManager::Update()
	{
		// '\' ������ ġƮ �� �� ���� ��
		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::KeyNum0))
		{
			SelectCheatCodeWithin();
		}

		//�Ϻ� �Լ� ȣ�� ���� ����.
		if (!_subFunctionStorageVector.empty())
		{
			//���� �ʾ��� ���� ����.
			for (auto& it : _subFunctionStorageVector)
			{
				//���ݱ��� �Ϻ� ������Ʈ���� ȣ���� �Լ��� �ϰ������� ����.
				it();
			}

			_subFunctionStorageVector.clear();
		}

		//�׿� �߰� ��ɵ�.
	}

	void TotalGameManager::OnSceneChange_Global(Pg::Data::Scene* changedScene)
	{
		// ���������� ��� �� ��Ʈ���ؾ� �Ѵ�.
		// �켱 �ʿ��� ���� Initialize.
		//�̷� ������ �������� Awake �� Function ����.
		if (!_isManagingInitializeCalled)
		{
			Initialize(changedScene);
			_isManagingInitializeCalled = true;
		}

		_pgTween->KillAllTweens();

		//�׽�Ʈ �������� ������� Scene�� �ƴ϶��.
		if ((changedScene->GetSceneName().compare("ToRemove_GraphicsTest") != 0) &&
			(changedScene->GetSceneName().compare("ToRemove_GraphicsTest.pgscene") != 0))
		{
			//���� Handler Bundle �޴´�. 2D Scene�� ��� nullptr.
			SetCurrentHandlerBundle(changedScene);

			// �ش� Prop / �ٲ� ��� / �ٲ� �� ������Ʈ���� ��� ������ �� �־�� �ϰ�,
			// �÷��̾��� ���� �� ��ġ�� �����ؾ� �ϸ�.
			// AreaHandler / EnemyHandler / GUIHandler (Stage1 / Stage2 �� ���� ���� - ������� �������̽��� ���� ��������)
			// ���� Stage ���� �ٸ��� ������ �ؾ� �ϱ⿡, �������̽��� ���� ���� �ʼ��̴�.
			// ���������� �ٸ� UI ������Ʈ���� ������ ��, �̸� ������ �ϰ� �־�� �Ѵ�. �̸� ����.
			// �̸� ���� GrabManagedObjects() �� ���õǾ���.
			// Player Object���� ��ü ����.

			// �ʱ� ���·� �ٽ� �ǵ��� ����. �� ���۽� ����.
			// �̷� ������, Flow Control�� ����Ѵ�.
			Internal_CallForEntireSceneReset(changedScene, NULL, nullptr);

			//������ ��������� ȣ�������.
			//������ Ŭ������� ���� �ִ� Player / Monster / Projectile ���� ���� �������� ���µȴ�.
			//�̹� Clear�� ���¿����� ����.
			_combatSystem->ResetAll();

			//���� Handler�� �ٽ� �µ� ����, CombatSystem�� ��������.
			if (_currentHandlerBundle3d != nullptr)
			{
				// == 3D�϶���,
				RegisterCombatSystemUnits();
			}
		}
	}

	void TotalGameManager::Initialize(Pg::Data::Scene* changedScene)
	{
		// ������ ���۵ǰ�, �ڽ��� ���� �� �ִ� SceneList���� �˾ƾ� �Ѵ�.
		//// PgScene �ޱ�.
		_pgScene = &singleton<Pg::API::PgScene>();
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgTween = &singleton<Pg::API::Tween::PgTween>();


		// �ݵ�� �ش� Object�� Don't Destroy On Load ������ �Ǿ� �־�� �Ѵ�.
		assert(_object->GetDontDestroyOnLoad() && "XML���� �̷��� ���Ծ�� �Ѵ�");

		// Scene���� Bundle���� �޴´�.
		SetupBundlesForAllScenes();

		//2D / 3D ���� ������ ����.
		//���� �Ŵ����� ���� ������ TotalGameManager�� �����Ѵ�.

		//�ٸ� �Ŵ����� ����. ���� ������Ʈ�� �����Ϸ��� �ϰ� �ִ�.
		//_inGameManager = InGameManager::GetInstance(nullptr);
		
		//Combat System ��, �� ���� �Ŵ��� �ý��� �޾ƿ���.
		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void TotalGameManager::SetupBundlesForAllScenes()
	{
		//���� ���� ���������� �����ϳ�, 
		//�̹� ���� �� ����������: ���� GrabManagedObject�� ȣ��Ǿ�,
		//���� ������ �� �־�� �Ѵ�.
		//ProjectLoad -> EngineMain�� ������Ʈ�̱⿡, ������ ������ ��. 
		for (auto& bScene : _pgScene->GetProjectSceneList())
		{
			//�̸��� Sample Scene�̸� ��ŵ.
			if ((bScene->GetSceneName().compare("SampleScene") == 0) ||
				(bScene->GetSceneName().compare("SampleScene.pgscene") == 0) ||
				(bScene->GetSceneName().compare("ToRemove_GraphicsTest") == 0) ||
				(bScene->GetSceneName().compare("ToRemove_GraphicsTest.pgscene") == 0))
			{
				//SampleScene�̸� Handler���� ���� �ʴ´�.
				continue;
			}

			//3D Scene�� �޾Ƽ�, ���� �� ȣ���� �Ѵ�.
			if (!(bScene->GetIs3D()))
			{
				//�ٸ�, Title/Credits�� ��� '��ü ��Ʈ'�� ���� ���� �´�. 
				//������ : �� �� 2D Scene�̶�� ��.
				//2D ������ GUI���� �����ϸ� �� ���̴�.
				//GUIHandler���� ����!
				Pg::DataScript::BaseGUIHandler* tGUIHandler = nullptr;

				for (auto& bObj : bScene->GetObjectList())
				{
					if (tGUIHandler == nullptr)
					{
						tGUIHandler = bObj->GetComponent<BaseGUIHandler>();
					}

					if (tGUIHandler != nullptr)
					{
						break;
					}
				}

				//Scene �� GUI Handler�� �¾�.
				_scene2dHandlerBundleMap.insert(std::make_pair(bScene, tGUIHandler));
			}
			else
			{
				Pg::DataScript::BaseAreaHandler* tAreaHandler = nullptr;
				Pg::DataScript::BaseEnemyHandler* tEnemyHandler = nullptr;
				Pg::DataScript::BaseGUIHandler* tGUIHandler = nullptr;
				Pg::DataScript::PlayerHandler* tPlayerBattleBehavior = nullptr;

				//�� �޾Ƽ� ���� �����ϱ�. (Handler �ø���)
				for (auto& bObj : bScene->GetObjectList())
				{
					if (tAreaHandler == nullptr)
					{
						tAreaHandler = bObj->GetComponent<BaseAreaHandler>();
					}

					if (tEnemyHandler == nullptr)
					{
						tEnemyHandler = bObj->GetComponent<BaseEnemyHandler>();
					}

					if (tGUIHandler == nullptr)
					{
						tGUIHandler = bObj->GetComponent<BaseGUIHandler>();
					}

					if (tPlayerBattleBehavior == nullptr)
					{
						tPlayerBattleBehavior = bObj->GetComponent<PlayerHandler>();
					}

					if (tAreaHandler != nullptr &&
						tEnemyHandler != nullptr &&
						tGUIHandler != nullptr &&
						tPlayerBattleBehavior != nullptr)
					{
						//�� �ִٶ�� �Ѵٸ� ��μ� ���� �� �ִ�.
						break;
					}
				}

				assert((tAreaHandler != nullptr) && "Area Handler�� �ɸ��� 3D ���ε� ����");
				assert((tEnemyHandler != nullptr) && "Enemy Handler�� �ɸ��� 3D���ε� ����");
				assert((tGUIHandler != nullptr) && "GUI Handler�� �ɸ��� 3D���ε� ����");
				assert((tPlayerBattleBehavior != nullptr) && "Player�� �ɸ��� 3D���ε� ����");

				//�� ���������� Handler�� ��� ����.
				std::unique_ptr<HandlerBundle3D> tHandlerBundle = std::make_unique<HandlerBundle3D>();
				tHandlerBundle->_areaHandler = tAreaHandler;
				tHandlerBundle->_enemyHandler = tEnemyHandler;
				tHandlerBundle->_guiHandler = tGUIHandler;
				tHandlerBundle->_playerBehavior = tPlayerBattleBehavior;

				//���� ������ ���� �Ҽ��� HandlerBundle3D �ּ� ���������� ����.
				// Area & Enemy.
				tHandlerBundle->_areaHandler->_belongHandlerBundle3D = tHandlerBundle.get();
				tHandlerBundle->_enemyHandler->_belongHandlerBundle3D = tHandlerBundle.get();


				//�ʱ� Player Position ���.
				tHandlerBundle->_originalPlayerTransStorage = TransformSimpleStorage(&(tPlayerBattleBehavior->_object->_transform));

				//���� ������ ������ ����.
				_scene3dHandlerBundleMap.insert(std::make_pair(bScene, std::move(tHandlerBundle)));
			}
		}
	}

	void TotalGameManager::SetCurrentHandlerBundle(Pg::Data::Scene* changedScene)
	{
		if (changedScene->GetIs3D())
		{
			//3D :���� Scene�� �����ϴ� Handler Bundle�� ����.
			_currentHandlerBundle3d = _scene3dHandlerBundleMap.at(changedScene).get();
			_currentGUIHander2d = nullptr;
		}
		else
		{
			//2D : �� Scene�� �����ϴ� GUI Handle�� ����.
			_currentGUIHander2d = _scene2dHandlerBundleMap.at(changedScene);
			_currentHandlerBundle3d = nullptr;
		}
	}

	void TotalGameManager::RegisterCombatSystemUnits()
	{
		_combatSystem->RegisterPlayer(_currentHandlerBundle3d->_playerBehavior);
		
		//���������� std::transformó�� ���� ȣ��� ��. std::function<void(IEnemyBehaviour*)>�� ������ ���̴�.
		_currentHandlerBundle3d->_enemyHandler->TransformEachEnemy(std::bind(&CombatSystem::RegisterSingleEnemy, _combatSystem, std::placeholders::_1));
	
		//Projectile�� �������� �ڽ��� CombatSystem���� ����� �ؾ� �� ���̴�.
	}

	void TotalGameManager::SelectCheatCodeWithin()
	{
		//�齽������ �ԷµǾ��� ���� ���ο� ���´�.
		// '\' + NUM ������ ��.
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyNum1))
		{
			_pgScene->SetCurrentScene("Stage1");
		}
		else if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyNum2))
		{
			_pgScene->SetCurrentScene("Stage2");
		}
		else if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyNum3))
		{
			_pgScene->SetCurrentScene("BossStage");
		}
		else if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyNum9))
		{
			_pgScene->SetCurrentScene("ToRemove_GraphicsTest");
		}

		//
	}

	void TotalGameManager::CallForEntireSceneReset(Pg::Data::Scene* targetScene, int potValue, void* potPointer)
	{
		//���߿� �ϰ��� ȣ���� �� �ֵ���, �Լ� ������ ����._pgSc3ne->GetKeyDown
		_subFunctionStorageVector.push_back([this, targetScene, potValue, potPointer]()
			{
				Internal_CallForEntireSceneReset(targetScene, potValue, potPointer);
			});
	}

	void TotalGameManager::Internal_CallForEntireSceneReset(Pg::Data::Scene* targetScene, int potValue, void* potPointer)
	{
		//��·�� 2D / 3D ���� �����ؼ� �����ؾ� �Ѵ�.
		if (_currentHandlerBundle3d == nullptr)
		{
			//2D ����.
			assert(_currentGUIHander2d != nullptr);
			_currentGUIHander2d->ResetToInitialState();
			_currentGUIHander2d->AdditionalReset();
		}
		else
		{
			//3D ����.
			_currentHandlerBundle3d->_areaHandler->ResetToInitialState();
			_currentHandlerBundle3d->_enemyHandler->ResetToInitialState();
			_currentHandlerBundle3d->_guiHandler->ResetToInitialState();
			_currentHandlerBundle3d->_guiHandler->AdditionalReset();

			//Player ��ġ ����ġ.
			_currentHandlerBundle3d->_originalPlayerTransStorage.ToTransform(&(_currentHandlerBundle3d->_playerBehavior->_object->_transform));
			_currentHandlerBundle3d->_playerBehavior->ResetAll();
		}
	}

	Pg::DataScript::HandlerBundle3D* TotalGameManager::GetCurrentHandlerBundle()
	{
		return _currentHandlerBundle3d;
	}

	Pg::DataScript::HandlerBundle3D* TotalGameManager::GetHandlerBundleByScene(Pg::Data::Scene* scene)
	{
		if (scene->GetIs3D())
		{
			return _scene3dHandlerBundleMap.at(scene).get();
		}
		else
		{
			return nullptr;
		}
	}
	

}
