#include "PgSound.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

namespace Pg::API::Sound
{
	void PgSound::Initialize()
	{

	}

	void PgSound::Play()
	{
		_audioSource->Play();
	}

	void PgSound::Stop()
	{
		_audioSource->Stop();
	}

	void PgSound::SetBGMVolume(float vol)
	{
		_audioSource->SetBGMVolume(vol);
	}

	void PgSound::SetEffectVolume(float vol)
	{
		_audioSource->SetEffectVolume(vol);
	}
}