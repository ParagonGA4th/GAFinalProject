#pragma once
#include "../ParagonData/AudioSource.h"

namespace Pg::Data
{
	class AudioSource;
}

namespace Pg::API::Sound
{
	class PgSound
	{

	public:
		void Initialize();

		void Play();

		void Stop();

		void SetBGMVolume(float vol);
		void SetEffectVolume(float vol);

	private:
		Pg::Data::AudioSource* _audioSource;
	};
}

