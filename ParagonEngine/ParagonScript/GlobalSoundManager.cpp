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

		//�ٲ� �� ����.
		_currentScene = changedScene;

	}

	void GlobalSoundManager::Initialize(Pg::Data::Scene* changedScene)
	{
		_pgScene = &singleton<Pg::API::PgScene>();
		// �ݵ�� �ش� Object�� Don't Destroy On Load ������ �Ǿ� �־�� �Ѵ�.
		assert(_object->GetDontDestroyOnLoad() && "XML���� �̷��� ���Ծ�� �Ѵ�");

		for (auto& bScene : _pgScene->GetProjectSceneList())
		{
			//�̸��� Sample Scene�̸� ��ŵ.
			if ((bScene->GetSceneName().compare("SampleScene") == 0) ||
				(bScene->GetSceneName().compare("SampleScene.pgscene") == 0))
			{
				//SampleScene�̸� Handler���� ���� �ʴ´�.
				continue;
			}

			//Scene���� Iterator�� �־�� �Ѵ�.
			auto it = _managedAudioSourceList.insert(std::make_pair(bScene, std::map<Pg::Data::eSoundGroup,
				std::vector<Pg::Data::AudioSource*>>()));
			auto& bSingleSceneMap = it.first->second;

			//BGM/InGame/Effect Iterator ����.
			bSingleSceneMap.insert(std::make_pair(Pg::Data::eSoundGroup::BGM, std::vector<Pg::Data::AudioSource*>()));
			bSingleSceneMap.insert(std::make_pair(Pg::Data::eSoundGroup::InGame, std::vector<Pg::Data::AudioSource*>()));
			bSingleSceneMap.insert(std::make_pair(Pg::Data::eSoundGroup::Effect, std::vector<Pg::Data::AudioSource*>()));

			//Object���� �ֱ�.
			for (auto& bObj : bScene->GetObjectList())
			{
				Pg::Data::AudioSource* tAudioSource = bObj->GetComponent<Pg::Data::AudioSource>();

				if (tAudioSource != nullptr)
				{
					//���� �� �߰��ؾ� �Ѵ�.
					//�׷캰�� �� ����.
					assert((tAudioSource->_audioData != nullptr) && "�̸� �� �ε��Ǿ� �־�� �ߴ� - Project");
					bSingleSceneMap.at(tAudioSource->_audioData->group).push_back(tAudioSource);
				}
			}
		}


	}

	void GlobalSoundManager::PauseSoundGroup(Pg::Data::eSoundGroup groupCh)
	{
		auto& tOuterMap = _managedAudioSourceList.at(_currentScene);
		auto& tActualBoundsMap = tOuterMap.at(groupCh);

		//�� Pause ȣ��.
		std::for_each(tActualBoundsMap.begin(), tActualBoundsMap.end(), [](Pg::Data::AudioSource* it)
			{
				it->Pause();
			});
	}

	void GlobalSoundManager::StopSoundGroup(Pg::Data::eSoundGroup groupCh)
	{
		auto& tOuterMap = _managedAudioSourceList.at(_currentScene);
		auto& tActualBoundsMap = tOuterMap.at(groupCh);

		//�� Pause ȣ��.
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
