#include "PgAudio.h"
#include "../ParagonGameEngine/SoundSystem.h"
#include "../ParagonGameEngine/PhysicSystem.h"

#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

namespace Pg::API::Sound
{
	void PgAudio::Initialize()
	{
		auto& tSoundSystem = singleton<Pg::Engine::SoundSystem>();
		_soundSystem = &tSoundSystem;
	}

	void PgAudio::SetBGMVolume(float vol)
	{
		_soundSystem->SetBGMVolume(vol);
	}

	void PgAudio::SetEffectVolume(float vol)
	{
		_soundSystem->SetEffectVolume(vol);
	}

	void PgAudio::SetIngameVolume(float vol)
	{
		_soundSystem->SetIngameVolume(vol);
	}

	float PgAudio::GetBGMVolume()
	{
		return _soundSystem->GetBGMVolume();
	}

	float PgAudio::GetIngameVolume()
	{
		return _soundSystem->GetIngameVolume();
	}

	float PgAudio::GetEffectVolume()
	{
		return _soundSystem->GetEffectVolume();
	}
}