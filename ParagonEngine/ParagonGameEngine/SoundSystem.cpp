#include "SoundSystem.h"
#include "SceneSystem.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/eSoundState.h"
#include "../ParagonData/Transform.h"
#include "../ParagonUtil/rapidcsv.h"
#include "../ParagonUtil/ResourceHelper.h"
#include "../ParagonUtil/CSVHelper.h"

#include <cassert>
#include <algorithm>
#include <singleton-cpp/singleton.h>

namespace Pg::Engine
{
	SoundSystem::SoundSystem() :
		_system(),
		_channelGroup(),
		_channelGroupVec(),
		_soundMap(),
		_maxSound(),
		_maxGroup(3)
	{
		//FMOD 초기화
		FMOD::System_Create(&_system);

		//시스템 초기화 (채널 설정을 동시에 해주어야 함)
		_system->init(100, FMOD_INIT_NORMAL, NULL);
		//채널 그룹 초기화
		_system->getMasterChannelGroup(&_channelGroup);


		// FMOD::ChannelGroup 초기화
		for (int i = 0; i < (int)_maxGroup; ++i) 
		{
			Pg::Data::eSoundGroup soundGroup = static_cast<Pg::Data::eSoundGroup>(i);

			// 개별 그룹 초기화
			_channelGroupVec[soundGroup] = nullptr;
			std::string groupName = "GroupName" + std::to_string(i);
			_system->createChannelGroup(groupName.c_str(), &_channelGroupVec[soundGroup]);

			_channelGroup->addGroup(_channelGroupVec[soundGroup]);
		}

		//3D 사운드의 크기나 거리 등을 설정
		//_system->set3DSettings(1.0f, 1.0f, 1.0f);

	}

	void SoundSystem::Initialize(const std::string& resourceListPath)
	{
		CreateSingleSounds(resourceListPath);

		//얘는 Scene이 바뀔때마다 호출되어야 함.
		SyncAudioSources();
	}


	void SoundSystem::Update()
	{
		//사운드의 재생 상태를 업데이트해서 받아옴.
		UpdateSounds();

		//사운드 들림.
		_system->update();
	}

	void SoundSystem::Finalize()
	{
		//채널 그룹 싹 다 해제.
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
		//사운드 객체 생성
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
		//무조건 볼륨 설정 후 재생.
		SetAllGroupVolume();

		for (auto& iter : _audioSoureceMap)
		{
			Pg::Data::AudioSource*& audioSource = iter.second;
			Pg::Data::AudioData*& audioData = audioSource->_audioData;
			Pg::Data::eSoundState& tSoundState = audioSource->_soundState;

			if (tSoundState == Pg::Data::eSoundState::_PLAY)
			{
				audioData->channel->setPaused(false);

				_system->playSound(audioData->sound, NULL, false, &audioData->channel);

				//소리 중복 재생을 위해 상태를 NONE으로 바꿔줌.
				tSoundState = Data::eSoundState::_NONE;
			}
			else if (tSoundState == Pg::Data::eSoundState::_STOP)
			{
				//_system->stopSound(audioData.sound, NULL, true, &audioData.channel);
				audioData->channel->stop();
				
				//소리 중복 재생을 위해 상태를 NONE으로 바꿔줌.
				tSoundState = Data::eSoundState::_NONE;
			}
			else if (tSoundState == Pg::Data::eSoundState::_PAUSE)
			{
				audioData->channel->setPaused(true);

				//소리 중복 재생을 위해 상태를 NONE으로 바꿔줌.
				tSoundState = Data::eSoundState::_NONE;
			}

		}
	}


	void SoundSystem::SetAllGroupVolume()
	{

		for (auto iter = _audioSoureceMap.begin(); iter != _audioSoureceMap.end(); iter++)
		{
			Pg::Data::AudioSource*& audioSource = iter->second;
			Pg::Data::AudioData*& audioData = audioSource->_audioData;

			if (audioData->group == Pg::Data::eSoundGroup::BGM)
			{
				audioData->channel->setVolume(audioSource->GetBGMVolume());
				//_channelGroupVec[soundGroup]->setVolume(audioSource->GetBGMVolume());
			}
			else if (audioData->group == Pg::Data::eSoundGroup::Effect)
			{
				audioData->channel->setVolume(audioSource->GetEffectVolume());
			}
		}
	}
	 
	void SoundSystem::SetGroupVolume(Pg::Data::eSoundGroup soundGroup)
	{
		for (auto iter = _audioSoureceMap.begin(); iter != _audioSoureceMap.end(); iter++)
		{
			Pg::Data::AudioSource*& audioSource = iter->second;
			Pg::Data::AudioData*& audioData = audioSource->_audioData;

			if (audioData->group == soundGroup)
			{
				audioData->channel->setVolume(audioSource->GetBGMVolume());
				_channelGroupVec[soundGroup]->setVolume(audioSource->GetBGMVolume());
			}
		}
	}


	void SoundSystem::SetAllVolume()
	{
		//사운드 객체 생성
		//for (auto& iter : _soundMap)
		//{
		//	Pg::Data::AudioData& audioData = iter.second;
		//	audioData.channel->setVolume(_audioSource->GetVolume());
		//}

		for (auto& iter : _audioSoureceMap)
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

		//여기다가 전부 사운드 만들어줘야함. 조절은 AudioSource에서.
		///사운드 파일은 엑셀의 csv 파일에 적을 예정.
		//CreateSound("../Resources/Sounds/Test/TitleBGM.mp3", eSoundGroup::BGM, false);
		//CreateSound("../Resources/Sounds/Test/Ingame.mp3", eSoundGroup::BGM, false);
		//CreateSound("../Resources/Sounds/Test/jump.mp3", eSoundGroup::Effect, false);

		_maxSound = _soundMap.size();
	}

	void SoundSystem::SyncAudioSources()
	{
		//원래 있던 AudioSourceList();
		if (!_audioSoureceMap.empty())
		{
			_audioSoureceMap.clear();
		}
		
		//싱글턴
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		for (auto& it : _sceneSystem->GetCurrentScene()->GetObjectList())
		{
			Pg::Data::AudioSource* tAudioSource = it->GetComponent<Pg::Data::AudioSource>();

			if (tAudioSource != nullptr)
			{
				assert(!tAudioSource->GetAudioName().empty() && "AudioSource의 Audio Name이 비워져 있으면 안됨");
				auto tAudioDataIt = _soundMap.find(tAudioSource->GetAudioName());
				assert(tAudioDataIt != _soundMap.end() && "SoundMap 내부에서 들어온 String 값의 AudioData를 찾지 못함");

				tAudioSource->_audioData = tAudioDataIt->second;

				_audioSoureceMap.insert(std::make_pair(tAudioDataIt->second->soundPath, tAudioSource));
			}

		}

		assert("");
	}

}