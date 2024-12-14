#include "SoundSystem.h"
#include "SceneSystem.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/eSoundState.h"
#include "../ParagonData/Transform.h"
#include "../ParagonHelper/rapidcsv.h"
#include "../ParagonHelper/ResourceHelper.h"
#include "../ParagonHelper/CSVHelper.h"

#include <cassert>
#include <algorithm>
#include <filesystem>
#include <singleton-cpp/singleton.h>

namespace Pg::Engine
{
	SoundSystem::SoundSystem() :
		_system(),
		_channelGroup(),
		_channelGroupVec(),
		_soundMap(),
		_maxSound(),
		_maxGroup(3),
		_bgmVolume(1.f),
		_ingameVolume(1.f),
		_effectVolume(0.7f)
	{
		//FMOD �ʱ�ȭ
		FMOD::System_Create(&_system);

		//�ý��� �ʱ�ȭ (ä�� ������ ���ÿ� ���־�� ��)
		_system->init(100, FMOD_INIT_NORMAL, NULL);
		//ä�� �׷� �ʱ�ȭ
		_system->getMasterChannelGroup(&_channelGroup);


		// FMOD::ChannelGroup �ʱ�ȭ
		for (int i = 0; i < (int)_maxGroup; ++i)
		{
			Pg::Data::eSoundGroup soundGroup = static_cast<Pg::Data::eSoundGroup>(i);

			// ���� �׷� �ʱ�ȭ
			_channelGroupVec[soundGroup] = nullptr;
			std::string groupName = "GroupName" + std::to_string(i);
			_system->createChannelGroup(groupName.c_str(), &_channelGroupVec[soundGroup]);

			_channelGroup->addGroup(_channelGroupVec[soundGroup]);
		}

		//3D ������ ũ�⳪ �Ÿ� ���� ����
		//_system->set3DSettings(1.0f, 1.0f, 1.0f);

	}

	SoundSystem::~SoundSystem()
	{

	}

	void SoundSystem::Initialize(const std::string& resourceListPath)
	{

		CreateSingleSounds(resourceListPath);

		//��� Scene�� �ٲ𶧸��� ȣ��Ǿ�� ��.
		SyncAudioSources();
	}


	void SoundSystem::Update()
	{
		//������ ��� ���¸� ������Ʈ�ؼ� �޾ƿ�.
		UpdateSounds();

		//���� �鸲.
		_system->update();
	}

	void SoundSystem::Finalize()
	{
		//ä�� �׷� �� �� ����.
		for (int i = 0; i < (int)_maxGroup; ++i)
		{
			Pg::Data::eSoundGroup soundGroup = static_cast<Pg::Data::eSoundGroup>(i);
			auto iter = _channelGroupVec.find(soundGroup);

			if (iter != _channelGroupVec.end())
			{
				iter->second->release();
			}
		}

		_channelGroup->release();
		_system->close();
		_system->release();
	}

	void SoundSystem::CreateSound(std::string path, Pg::Data::eSoundGroup soundGroup, bool isLoop)
	{
		//���� ��ü ����
		Pg::Data::AudioData* audioData = new Pg::Data::AudioData();

		if (!isLoop)
		{
			_system->createSound(path.c_str(), FMOD_LOOP_OFF | FMOD_DEFAULT, NULL, &(audioData->sound));
		}
		else
		{
			_system->createSound(path.c_str(), FMOD_LOOP_NORMAL | FMOD_DEFAULT, NULL, &(audioData->sound));
		}

		audioData->soundPath = path;
		audioData->group = soundGroup;
		audioData->channel = 0;
		audioData->channel->setChannelGroup(_channelGroupVec[soundGroup]);

		_soundMap.insert({ path, audioData });
	}

	void SoundSystem::PlaySound(std::string path)
	{

		_system->playSound(_soundMap.at(path)->sound, NULL, true, &_soundMap.at(path)->channel);
		_soundMap.at(path)->channel->setPaused(false);

		//for (int i = 0; i < (int)_maxSound; i++)
		//{
		//	if (_soundMap.at(path).soundPath == path)
		//	{
		//		_system->playSound(_soundMap.at(path).sound, NULL, true, &_soundMap.at(path).channel);
		//		_soundMap.at(path).channel->setPaused(false);
		//		_soundMap.at(path).isPlaying =
		//			_soundMap.at(path).channel->getCurrentSound(&_soundMap.at(path).sound);
		//	}
		//}
	}


	void SoundSystem::UpdateSounds()
	{
		//������ ���� ���� �� ���.
		SetAllGroupVolume();

		for (auto& iter : _audioSourceMap)
		{
			Pg::Data::AudioSource*& audioSource = iter.second;
			Pg::Data::AudioData*& audioData = audioSource->_audioData;
			Pg::Data::eSoundState& tSoundState = audioSource->_soundState;

			if (tSoundState == Pg::Data::eSoundState::_PLAY)
			{
				audioData->channel->setPaused(false);

				_system->playSound(audioData->sound, NULL, false, &audioData->channel);

				//�Ҹ� �ߺ� ����� ���� ���¸� NONE���� �ٲ���.
				tSoundState = Data::eSoundState::_NONE;
			}
			else if (tSoundState == Pg::Data::eSoundState::_STOP)
			{
				//_system->stopSound(audioData.sound, NULL, true, &audioData.channel);
				audioData->channel->stop();

				//�Ҹ� �ߺ� ����� ���� ���¸� NONE���� �ٲ���.
				tSoundState = Data::eSoundState::_NONE;
			}
			else if (tSoundState == Pg::Data::eSoundState::_PAUSE)
			{
				audioData->channel->setPaused(true);

				//�Ҹ� �ߺ� ����� ���� ���¸� NONE���� �ٲ���.
				tSoundState = Data::eSoundState::_NONE;
			}

		}
	}


	void SoundSystem::SetAllGroupVolume()
	{
		for (auto iter = _audioSourceMap.begin(); iter != _audioSourceMap.end(); iter++)
		{
			Pg::Data::AudioSource*& audioSource = iter->second;
			Pg::Data::AudioData*& audioData = audioSource->_audioData;

			if (audioData->group == Pg::Data::eSoundGroup::BGM)
			{
				audioData->channel->setVolume(_bgmVolume);
				//_channelGroupVec[soundGroup]->setVolume(audioSource->GetBGMVolume());
			}
			else if (audioData->group == Pg::Data::eSoundGroup::Effect)
			{
				audioData->channel->setVolume(_effectVolume);
			}
			else if (audioData->group == Pg::Data::eSoundGroup::InGame)
			{
				audioData->channel->setVolume(_ingameVolume);
			}
		}
	}

	void SoundSystem::SetAllVolume()
	{
		//���� ��ü ����
		//for (auto& iter : _soundMap)
		//{
		//	Pg::Data::AudioData& audioData = iter.second;
		//	audioData.channel->setVolume(_audioSource->GetVolume());
		//}

		for (auto& iter : _audioSourceMap)
		{
			Pg::Data::AudioSource*& audioSource = iter.second;
			Pg::Data::AudioData*& audioData = audioSource->_audioData;
			//audioData->channel->setVolume(audioSource->GetVolume());
		}
	}

	void SoundSystem::SoundPause(std::string path, bool isPause)
	{
		_soundMap.at(path)->channel->setPaused(true);
	}

	std::unordered_map<std::string, Pg::Data::AudioData*>& SoundSystem::GetSoundMap()
	{
		return _soundMap;
	}

	void SoundSystem::CreateSingleSounds(const std::string& resourceListPath)
	{
		std::string tUniformPath = Pg::Util::Helper::ResourceHelper::ForcePathUniformFull(resourceListPath);
		std::string tPath = tUniformPath + "/11_Sounds.csv";
		auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromSoundFileCSV(tPath);

		for (auto& [bFilePath, bSoundGroup, bLoopBool] : tPathVec)
		{
			CreateSound(bFilePath, bSoundGroup, bLoopBool);
		}

		//����ٰ� ���� ���� ����������. ������ AudioSource����.
		///���� ������ ������ csv ���Ͽ� ���� ����.
		//CreateSound("../Resources/Sounds/Test/TitleBGM.mp3", eSoundGroup::BGM, false);
		//CreateSound("../Resources/Sounds/Test/Ingame.mp3", eSoundGroup::BGM, false);
		//CreateSound("../Resources/Sounds/Test/jump.mp3", eSoundGroup::Effect, false);

		_maxSound = _soundMap.size();
	}

	void SoundSystem::SyncAudioSources()
	{
		//���� �ִ� AudioSourceList();
		if (!_audioSourceMap.empty())
		{
			for (auto& it : _audioSourceMap)
			{
				it.second->Stop();
			}

			//�� ���� �ִ� ���� ���߱�.
			UpdateSounds();

			_audioSourceMap.clear();
		}

		//�̱���
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		//�׳� �ٽ� ã�ƶ�.
		InsertSingleSceneAudioSources(_sceneSystem->GetCurrentScene());

		assert("");
	}

	void SoundSystem::SetBGMVolume(float vol)
	{
		_bgmVolume = vol;
	}

	float SoundSystem::GetBGMVolume()
	{
		return _bgmVolume;
	}

	void SoundSystem::SetIngameVolume(float vol)
	{
		_ingameVolume = vol;
	}

	float SoundSystem::GetIngameVolume()
	{
		return _ingameVolume;
	}

	void SoundSystem::SetEffectVolume(float vol)
	{
		_effectVolume = vol;
	}

	float SoundSystem::GetEffectVolume()
	{
		return _effectVolume;
	}

	void SoundSystem::LoadSoundListOnProjectLoad(const std::vector<Pg::Data::Scene*>& sceneVec)
	{
		//Project Load�� ���� Scene�鿡 �־�, ���������� AudioData�� �ִ� �� ��ü�� �߿�.
		for (auto& bProjScene : sceneVec)
		{
			InsertSingleSceneAudioSources(bProjScene);
		}

		// �� ��ü�� AudioData�� ä���ִ� �� ��ü�� ���ǰ� �־���. 
		// �׷��Ƿ�, ���� Scene�� ���� ����� �ƴϹǷ�, Clear.
		_audioSourceMap.clear();
	}

	void SoundSystem::InsertSingleSceneAudioSources(Pg::Data::Scene* scene)
	{
		for (auto& it : scene->GetObjectList())
		{
			Pg::Data::AudioSource* tAudioSource = it->GetComponent<Pg::Data::AudioSource>();

			if (tAudioSource != nullptr)
			{
				assert(!tAudioSource->GetAudioName().empty() && "AudioSource�� Audio Name�� ����� ������ �ȵ�");
				//auto tAudioDataIt = _soundMap.find(tAudioSource->GetAudioName());

				//������ �̸����� ã�´�.
				std::string name = tAudioSource->GetAudioName();
				Pg::Data::AudioData* tDataFound = nullptr;
				std::string tFullPath = "";
				for (auto& it : _soundMap)
				{
					std::filesystem::path tPath = it.first;
					std::string tVal = tPath.filename().string();

					if (tVal.compare(name) == 0)
					{
						//���� �̸��� ���ҽ��� ���� ���.
						tFullPath = it.first;
						tDataFound = it.second;
					}
				}
				//</>

				assert(tDataFound != nullptr && "SoundMap ���ο��� ���� String ���� AudioData�� ã�� ����");

				tAudioSource->_audioData = tDataFound;

				_audioSourceMap.insert(std::make_pair(tFullPath, tAudioSource));
			}

		}
	}

}