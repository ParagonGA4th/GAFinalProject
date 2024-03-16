#include "PgAudio.h"
#include "../ParagonGameEngine/SoundSystem.h"

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
		//auto& tSoundSystem = singleton<Pg::Engine::SoundSystem>();
		//_soundSystem = &tSoundSystem;
	}

	//void PgSound::SetBGMVolume(float vol)
	//{
	//	_soundSystem->SetBGMVolume(vol);
	//}

	//void PgSound::SetEffectVolume(float vol)
	//{
	//	_soundSystem->SetEffectVolume(vol);
	//}

	//void PgSound::SetIngameVolume(float vol)
	//{
	//	_soundSystem->SetIngameVolume(vol);
	//}

	//float PgSound::GetBGMVolume()
	//{
	//	return _soundSystem->GetBGMVolume();
	//}

	//float PgSound::GetIngameVolume()
	//{
	//	return _soundSystem->GetIngameVolume();
	//}

	//float PgSound::GetEffectVolume()
	//{
	//	return _soundSystem->GetEffectVolume();
	//}
}