#include "GlobalSoundManager.h"
#include "../ParagonAPI/PgScene.h"
#include <singleton-cpp/singleton.h>
#include <algorithm>

namespace Pg::DataScript
{
	void GlobalSoundManager::Awake()
	{

	}

	void GlobalSoundManager::Start()
	{

	}

	void GlobalSoundManager::Update()
	{

	}

	void GlobalSoundManager::OnSceneChange_Global(Pg::Data::Scene* changedScene)
	{
		if (!_isManagingInitializeCalled)
		{
			Initialize(changedScene);
			_isManagingInitializeCalled = true;
		}

		if (_currentScene != nullptr)
		{
			StopAllSoundGroups();
		}

		//바뀐 씬 저장.
		_currentScene = changedScene;

	}

	void GlobalSoundManager::Initialize(Pg::Data::Scene* changedScene)
	{
		_pgScene = &singleton<Pg::API::PgScene>();
		// 반드시 해당 Object는 Don't Destroy On Load 설정이 되어 있어야 한다.
		assert(_object->GetDontDestroyOnLoad() && "XML에서 이렇게 들어왔어야 한다");

		for (auto& bScene : _pgScene->GetProjectSceneList())
		{
			//이름이 Sample Scene이면 스킵.
			if ((bScene->GetSceneName().compare("SampleScene") == 0) ||
				(bScene->GetSceneName().compare("SampleScene.pgscene") == 0))
			{
				//SampleScene이면 Handler들을 받지 않는다.
				continue;
			}

			//Scene별로 Iterator를 넣어야 한다.
			auto it = _managedAudioSourceList.insert(std::make_pair(bScene, std::map<Pg::Data::eSoundGroup,
				std::vector<Pg::Data::AudioSource*>>()));
			auto& bSingleSceneMap = it.first->second;

			//BGM/InGame/Effect Iterator 투입.
			bSingleSceneMap.insert(std::make_pair(Pg::Data::eSoundGroup::BGM, std::vector<Pg::Data::AudioSource*>()));
			bSingleSceneMap.insert(std::make_pair(Pg::Data::eSoundGroup::InGame, std::vector<Pg::Data::AudioSource*>()));
			bSingleSceneMap.insert(std::make_pair(Pg::Data::eSoundGroup::Effect, std::vector<Pg::Data::AudioSource*>()));

			//Object별로 넣기.
			for (auto& bObj : bScene->GetObjectList())
			{
				Pg::Data::AudioSource* tAudioSource = bObj->GetComponent<Pg::Data::AudioSource>();

				if (tAudioSource != nullptr)
				{
					//없을 때 추가해야 한다.
					//그룹별로 값 투입.
					assert((tAudioSource->_audioData != nullptr) && "미리 다 로딩되어 있어야 했다 - Project");
					bSingleSceneMap.at(tAudioSource->_audioData->group).push_back(tAudioSource);
				}
			}
		}


	}

	void GlobalSoundManager::PauseSoundGroup(Pg::Data::eSoundGroup groupCh)
	{
		auto& tOuterMap = _managedAudioSourceList.at(_currentScene);
		auto& tActualBoundsMap = tOuterMap.at(groupCh);

		//다 Pause 호출.
		std::for_each(tActualBoundsMap.begin(), tActualBoundsMap.end(), [](Pg::Data::AudioSource* it)
			{
				it->Pause();
			});
	}

	void GlobalSoundManager::StopSoundGroup(Pg::Data::eSoundGroup groupCh)
	{
		auto& tOuterMap = _managedAudioSourceList.at(_currentScene);
		auto& tActualBoundsMap = tOuterMap.at(groupCh);

		//다 Pause 호출.
		std::for_each(tActualBoundsMap.begin(), tActualBoundsMap.end(), [](Pg::Data::AudioSource* it)
			{
				it->Stop();
			});
	}

	void GlobalSoundManager::PauseAllSoundGroups()
	{
		auto& tOuterMap = _managedAudioSourceList.at(_currentScene);

		for (auto& [bGroup, bVec] : tOuterMap)
		{
			for (auto& it : bVec)
			{
				it->Pause();
			}
		}
	}

	void GlobalSoundManager::StopAllSoundGroups()
	{
		auto& tOuterMap = _managedAudioSourceList.at(_currentScene);

		for (auto& [bGroup, bVec] : tOuterMap)
		{
			for (auto& it : bVec)
			{
				it->Stop();
			}
		}
	}

}
