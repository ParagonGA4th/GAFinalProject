#include "AudioSource.h"
#include "GameObject.h"
#include "eSoundState.h"
#include "AudioData.h"

#include "../ParagonGameEngine/SoundSystem.h"

#include <singleton-cpp/singleton.h>


namespace Pg::Data
{	
	AudioSource::AudioSource(GameObject* owner) :
		Component(owner),
		_bgmVolume(1.0f),
		_effectVolume(1.0f),
		_soundState(eSoundState::_NONE)
	{
		//Auto-Register µî·Ï.
		FACTORY_INIT;
	}

	void AudioSource::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void AudioSource::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void AudioSource::Start()
	{

	}

	void AudioSource::Play()
	{
		_soundState = eSoundState::_PLAY;                                  
	}

	void AudioSource::Stop()
	{
		_soundState = eSoundState::_STOP;
	}

	void AudioSource::Pause()
	{
		_soundState = eSoundState::_PAUSE;
	}

	void AudioSource::ResetPlayingState()
	{
		_soundState = eSoundState::_NONE;
	}

	//void AudioSource::SetBGMVolume(float vol)
	//{
	//	_bgmVolume = vol;
	//}

	//float AudioSource::GetBGMVolume()
	//{
	//	return _bgmVolume;
	//}

	//void AudioSource::SetEffectVolume(float vol)
	//{
	//	_effectVolume = vol;
	//}

	//float AudioSource::GetEffectVolume()
	//{
	//	return _effectVolume;
	//}

	void AudioSource::SetAudioName(const std::string& audioName)
	{
		_audioName = audioName;
	}

	std::string AudioSource::GetAudioName()
	{
		return _audioName;
	}
}