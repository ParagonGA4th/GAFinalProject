#include "SoundSystem.h"
#include "SceneSystem.h"
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

		//3D 사운드의 크기나 거리 등을 설정
		//_system->set3DSettings(1.0f, 1.0f, 1.0f);

	}

	void SoundSystem::Initialize()
	{
		//싱글턴
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		_maxSound = _soundMap.size();

		
	}


	void SoundSystem::Update()
	{
		for (auto& obj : _sceneSystem->GetCurrentScene()->GetObjectList())
		{
			//if ()
			//{
			//	//CreateSound()
			//}
		}
		_system->update();
	}

	void SoundSystem::Finalize()
	{
		//채널 그룹 싹 다 해제.
		for (int i = 0; i < _maxGroup; i++)
		{
			_channelGroupVec[i]->release();
		}

		_system->close();
		_system->release();
	}

	void SoundSystem::CreateSound(std::string path, bool isLoop)
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

		audioData.isPlaying = false;
		audioData.channel = 0;
		//audioData.channel->setChannelGroup(_channelGroupVec[_channelGroup]);
		audioData.soundPath = path;

		_soundMap.insert({ path, audioData });
	}

	void SoundSystem::PlaySound(std::string path)
	{
		for (int i = 0; i < (int)_maxSound; i++)
		{
			if (_soundMap.at(path).soundPath == path)
			{
				_system->playSound(_soundMap.at(path).sound, NULL, false, &_soundMap.at(path).channel);
				_soundMap.at(path).isPlaying = true;
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