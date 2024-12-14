#pragma once
#include "Component.h"

#include <string>
#include "data_factory.h" // Auto Register를 위한 필수요건.

namespace Pg::Engine
{
	class SoundSystem;
}

/// <summary>
/// 변지상의 AudioSource 컴포넌트.
/// void 포인터 쓰기 싫어!!!!ㅠㅠ
/// 2024.02.06
/// </summary>
namespace Pg::Data
{
	class GameObject;
	struct AudioData;
	enum class eSoundState;

	//														부모			자신		매개변수..
	class AudioSource : public Component, Pg::Factory::Data::RegisteredInFactory<Component, AudioSource, GameObject*>
	{
	public:
		AudioSource(GameObject* owner);

		//자동화된 Auto-Registration 작동 위해 필수.
		static Component* CreateInstance(GameObject* go) { return new AudioSource(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::AudioSource"; }

		virtual void Start() override;
		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		void Play();

		void Stop();
		
		void Pause();

		//재생 상태 싹 다 리셋.
		void ResetPlayingState();

		void SetAudioName(const std::string& audioName);
		std::string GetAudioName();

		//void SetBGMVolume(float vol);
		//void SetEffectVolume(float vol);

		//float GetBGMVolume();
		//float GetEffectVolume();
	public:
		//재생 상태를 나타내는 클래스.
		//조절은 API에서 해줄꺼임.
		AudioData* _audioData;
		eSoundState _soundState;

		BEGIN_VISITABLES(Pg::Data::AudioSource);
		VISITABLE(std::string, _audioName);
		END_VISITABLES;

	private:
		float _bgmVolume;
		float _effectVolume;
	};
}