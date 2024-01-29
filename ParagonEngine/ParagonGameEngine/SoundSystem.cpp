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