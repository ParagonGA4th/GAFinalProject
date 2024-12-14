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

		//Scene이 바뀔 때 Global Object만 루프로 호출됨. 이제 호출 순서도 정해져 있으니, 걱정 안해도 됨.
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) override;

		//Manager들 중에서도 가장 먼저 Update가 돌아야 한다.
		virtual unsigned int GetPriorityIndex() override { return 3; }
		
	public:
		//외부 호출 함수용.
		void PauseSoundGroup(Pg::Data::eSoundGroup groupCh);
		void StopSoundGroup(Pg::Data::eSoundGroup groupCh);
		void PauseAllSoundGroups();			   // 얘네는 씬이 바뀔 때에는 제 기능을 못할 것이다.
		void StopAllSoundGroups();			   // 얘네는 씬이 바뀔 때에는 제 기능을 못할 것이다.


	private:
		void Initialize(Pg::Data::Scene* changedScene);
		

	private:
		Pg::API::PgScene* _pgScene{ nullptr };
		Pg::Data::Scene* _currentScene{ nullptr };
	private:
		//Game 내 존재하는 모든 Sound를 관리.
		std::unordered_map<Pg::Data::Scene*, 
			std::map<Pg::Data::eSoundGroup, 
			std::vector<Pg::Data::AudioSource*>>> _managedAudioSourceList;

		//전체 값이 받아졌는지 -> Initialize를 초기에 하기 위해서.
		bool _isManagingInitializeCalled{ false };

	private:
		
	};
}


