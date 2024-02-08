#include "AudioSource.h"
#include "GameObject.h"
#include "eSoundState.h"

#include "../ParagonGameEngine/SoundSystem.h"

#include <singleton-cpp/singleton.h>

namespace Pg::Data
{
	AudioSource::AudioSource(GameObject* owner) :
		Component(owner)
	{
		
	}

	void AudioSource::Start()
	{

	}

	void AudioSource::Play(std::string audioName)
	{
		
	}

	void AudioSource::Stop(std::string audioName)
	{

	}

	void AudioSource::Pause(std::string audioName)
	{

	}

	void AudioSource::ResetPlayingState()
	{
		_soundState = eSoundState::_NONE;
	}
}