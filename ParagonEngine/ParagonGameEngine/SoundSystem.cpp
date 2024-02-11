#include "SoundSystem.h"
#include "SceneSystem.h"
#include "eSoundGroup.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/eSoundState.h"
#include "../ParagonData/Transform.h"

namespace Pg::Engine
{
	SoundSystem::SoundSystem() :
		_system(),
		_channelGroup()
	{
		//FMOD 초기화
		FMOD::System_Create(&_system);

		//시스템 초기화 (채널 설정을 동시에 해주어야 함)
		_system->init(100, FMOD_INIT_NORMAL, NULL);
		//채널 그룹 초기화
		_system->getMasterChannelGroup(&_channelGroup);


		// FMOD::ChannelGroup 초기화
		for (int i = 0; i < (int)_maxGroup; i++)
		{
			// 개별 Group 초기화
			_channelGroupVec[_eSoundGroup] = 0;
			std::string temp = "GroupName" + std::to_string(i);
			_system->createChannelGroup(temp.c_str(), &_channelGroupVec.at(_eSoundGroup));
		}

		//3D 사운드의 크기나 거리 등을 설정
		//_system->set3DSettings(1.0f, 1.0f, 1.0f);

	}

	void SoundSystem::Initialize()
	{
		//싱글턴
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		_maxSound = _soundMap.size();

		//test
		CreateSound("../Resources/Sounds/Test/TitleBGM", eSoundGroup::BGM, false);
		SetVolume("../Resources/Sounds/Test/TitleBGM", 1.0f);
		
	}


	void SoundSystem::Update()
	{
		//if (_audioSource->_soundState == Pg::Data::eSoundState::_PLAY)
		//{
		//	PlaySound("../Sounds/Test/TitleBGM");
		//}
		_system->update();

		//test
		PlaySound("../Resources/Sounds/Test/TitleBGM");
	}

	void SoundSystem::Finalize()
	{
		//채널 그룹 싹 다 해제.
		//for (int i = 0; i < _maxGroup; i++)
		//{
		//	//_channelGroupVec->release();
		//	_channelGroupVec[i].clear();
		//}

		for (auto& iter : _channelGroupVec)
		{
			iter.second->release();
		}

		_channelGroup->release();
		_system->close();
		_system->release();
	}

	void SoundSystem::CreateSound(std::string path, eSoundGroup soundGroup, bool isLoop)
	{
		//사운드 객체 생성
		Pg::Data::AudioData audioData;

		if (!isLoop)
		{
			_system->createSound(path.c_str(), FMOD_LOOP_OFF | FMOD_DEFAULT, NULL, &(audioData.sound));
		}
		else
		{
			_system->createSound(path.c_str(), FMOD_LOOP_NORMAL | FMOD_DEFAULT, NULL, &(audioData.sound));
		}

		audioData.soundPath = path;
		audioData.isPlaying = true;
		audioData.channel = 0;
		audioData.channel->setChannelGroup(_channelGroupVec[soundGroup]);

		_soundMap.insert({ path, audioData });
	}

	void SoundSystem::PlaySound(std::string path)
	{
		for (int i = 0; i < (int)_maxSound; i++)
		{
			//_system->playSound(_soundMap.begin(), NULL, false, &_soundMap.at(path).channel);
			//_soundMap.at(path).isPlaying = true;

			if (_soundMap.at(path).soundPath == path)
			{
				_system->playSound(_soundMap.at(path).sound, NULL, true, &_soundMap.at(path).channel);
				_soundMap.at(path).channel->setPaused(false);
				_soundMap.at(path).isPlaying = 
					_soundMap.at(path).channel->getCurrentSound(&_soundMap.at(path).sound);;
			}
		}
	}

	void SoundSystem::SetVolume(std::string path, float vol)
	{
		_soundMap.at(path).channel->setVolume(vol);
	}

	void SoundSystem::SoundPause(std::string path, bool isPause)
	{
		_soundMap.at(path).channel->setPaused(true);
	}

	std::unordered_map<std::string, Pg::Data::AudioData>& SoundSystem::GetSoundMap()
	{
		return _soundMap;
	}

}