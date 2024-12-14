#pragma once

#pragma once
#include "ScriptInterface.h"
#include "GameState.h"
#include "HandleBundle3D.h"
#include "IObserver.h"

#include "../ParagonData/ISortableGlobalObject.h"

#include <string>
#include <vector>

/// <summary>
///	Total�� �÷ο� �������, InGameManager�� 
/// �Ĺ� �� ���� ���θ� �Ѱ��ؼ� �����Ѵ�.
///  
/// </summary>

namespace Pg::API::Time { class PgTime; }
namespace Pg::API::Input { class PgInput; }
namespace Pg::API { class PgScene; }

namespace Pg::DataScript
{
	class TotalGameManager;
	class CombatSystem;

	class InGameManager : public ScriptInterface<InGameManager>, public Pg::Data::ISortableGlobalObject, public IObserver
	{
		DEFINE_PARAGON_SCRIPT_SINGLETON(InGameManager);

	public:
		static const int MAX_MONSTER_COUNT_IN_LEVEL = 100;

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void LateUpdate() override;

		//Scene�� �ٲ� �� Global Object�� ������ ȣ���.
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) override;
		
		//TotalGameManager �������� ȣ��.
		virtual unsigned int GetPriorityIndex() override { return 1; }

		//IObserver : HandleEvents.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

	public:
		bool GetEnableGameManagerUpdate();

	private:
		//Initialize.
		void Initialize(Pg::Data::Scene* changedScene);

		void RegisterToCombatSystemEvents();
		// Logic ������Ʈ, 3D�� ��쿡.
		// �ۿ��� ������ ���� ����.

	public:
		void ChangeMonsterKillCount(int level);
	private:
		int _numberOfMonstersKilled{ 0 };					// ���� ������ ����, Scene �ٲ� / ���� �ø��� �ٲ� ��.

	private:
		float _gameTime;				// ���� �� �ð�
		eGameState _gameState;			// ���� ���� 
		//std::string _currentSceneName;	// ���� ��
		//std::vector<std::string> _sceneList; // ������ ���

		
		ePlayerState _playerState;		// �÷��̾��� ����
		std::vector<std::pair<std::string, bool>> _artifacts;	// �÷��̾ ���� ���� ��Ƽ��Ʈ

		/// Stage
		
		
	
		

	private:
		//��ü ���� �޾������� -> Initialize�� �ʱ⿡ �ϱ� ���ؼ�.
		bool _isManagingInitializeCalled{ false };

		//TotalGameManager�� ���� Update Mode Set.
		bool _isUpdating{ false };

	private:
		//API
		Pg::API::PgScene* _pgScene{ nullptr };
		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::API::Time::PgTime* _pgTime{ nullptr };

		//Total Game Manager ��, ��Ÿ �Ŵ����� ����.
		TotalGameManager* _totalGameManager{ nullptr };

		//���� 3D�� ���, ������ nullptr�� �ƴ�. �ƴ� ���� nullptr �´�.
		HandlerBundle3D* _handlerBundle3D{ nullptr };

		CombatSystem* _combatSystem{ nullptr };

		//���� �ڽ��� ���� �־��� Scene.
		Pg::Data::Scene* _recordedPreviousScene{ nullptr };
		

	};
}


