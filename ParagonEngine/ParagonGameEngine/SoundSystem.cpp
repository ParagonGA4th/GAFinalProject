#include "SoundSystem.h"

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

	}

	void SoundSystem::Finalize()
	{

	}

	void SoundSystem::CreateSound(std::string path)
	{

	}

}