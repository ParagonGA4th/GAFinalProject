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
/// ��ũ���� �� �ʿ��� ���� ���� �Լ� API.
/// ������
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
