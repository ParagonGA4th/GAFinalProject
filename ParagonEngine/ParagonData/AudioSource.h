#pragma once
#include "Component.h"

#include <string>

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

	class AudioSource : public Component
	{
	public:
		AudioSource(GameObject* owner);

		virtual void Start() override;

		void Play();

		void Stop();
		
		void Pause();

		void SetBGMVolume(float vol);
		float GetBGMVolume();

		void SetEffectVolume(float vol);
		float GetEffectVolume();

		//재생 상태 싹 다 리셋.
		void ResetPlayingState();

		void SetAudioName(const std::string& audioName);
		std::string GetAudioName();
	public:
		//재생 상태를 나타내는 클래스.
		//조절은 API에서 해줄꺼임.
		AudioData* _audioData;
		eSoundState _soundState;

		std::string _audioName;


	private:
		float _bgmVolume;
		float _effectVolume;

	};
}