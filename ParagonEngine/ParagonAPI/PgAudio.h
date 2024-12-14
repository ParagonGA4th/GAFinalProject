#pragma once

namespace Pg::Engine
{
	class SoundSystem;

	namespace Physic
	{
		class PhysicSystem;
	}
}

/// <summary>
/// 스크립팅 시 필요한 사운드 관련 함수 API.
/// 변지상
/// </summary>
namespace Pg::API::Sound
{
	class PgAudio
	{
	public:
		void Initialize();

		void SetBGMVolume(float vol);
		void SetEffectVolume(float vol);
		void SetIngameVolume(float vol);

		float GetBGMVolume();
		float GetIngameVolume();
		float GetEffectVolume();

	private:
		Pg::Engine::SoundSystem* _soundSystem = nullptr;
	};
}
