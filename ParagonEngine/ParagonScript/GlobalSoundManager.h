#pragma once
#include "ScriptInterface.h"
#include "GameState.h"
#include "HandleBundle3D.h"

#include "../ParagonData/ISortableGlobalObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/AudioData.h"
#include "../ParagonData/eSoundGroup.h"

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <memory>
#include <functional>

namespace Pg::API { class PgScene; }

namespace Pg::DataScript
{
	class GlobalSoundManager : public ScriptInterface<GlobalSoundManager>, public Pg::Data::ISortableGlobalObject
	{
		DEFINE_PARAGON_SCRIPT_SINGLETON(GlobalSoundManager);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		//Scene�� �ٲ� �� Global Object�� ������ ȣ���. ���� ȣ�� ������ ������ ������, ���� ���ص� ��.
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) override;

		//Manager�� �߿����� ���� ���� Update�� ���ƾ� �Ѵ�.
		virtual unsigned int GetPriorityIndex() override { return 3; }
		
	public:
		//�ܺ� ȣ�� �Լ���.
		void PauseSoundGroup(Pg::Data::eSoundGroup groupCh);
		void StopSoundGroup(Pg::Data::eSoundGroup groupCh);
		void PauseAllSoundGroups();			   // ��״� ���� �ٲ� ������ �� ����� ���� ���̴�.
		void StopAllSoundGroups();			   // ��״� ���� �ٲ� ������ �� ����� ���� ���̴�.


	private:
		void Initialize(Pg::Data::Scene* changedScene);
		

	private:
		Pg::API::PgScene* _pgScene{ nullptr };
		Pg::Data::Scene* _currentScene{ nullptr };
	private:
		//Game �� �����ϴ� ��� Sound�� ����.
		std::unordered_map<Pg::Data::Scene*, 
			std::map<Pg::Data::eSoundGroup, 
			std::vector<Pg::Data::AudioSource*>>> _managedAudioSourceList;

		//��ü ���� �޾������� -> Initialize�� �ʱ⿡ �ϱ� ���ؼ�.
		bool _isManagingInitializeCalled{ false };

	private:
		
	};
}


