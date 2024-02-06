#include "SoundSystem.h"
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
		_system->set3DSettings(1.0f, 1.0f, 1.0f);

	}

	void SoundSystem::Initialize()
	{
		_maxSound = _soundMap.size();
	}


	void SoundSystem::Update()
	{
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

	void SoundSystem::CreateSound(std::string path)
	{
		//사운드 객체 생성
		Pg::Data::AudioData audioData;

		_system->createSound(path.c_str(), FMOD_LOOP_NORMAL | FMOD_DEFAULT, NULL, &(audioData.sound));
	}

	void SoundSystem::PlaySound(std::string path, bool isLoop)
	{

	}

	void SoundSystem::SetVolume(std::string path, float vol)
	{

	}

	void SoundSystem::SoundPause(std::string path)
	{

	}
}