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

	class AudioSource : public Component
	{
		AudioSource(GameObject* owner);

	public:
		virtual void Start() override;
		void Play(std::string audioName);

		void Stop(std::string audioName);
		
		void Pause(std::string audioName);

		//재생 상태 싹 다 리셋.
		void ResetPlayingState();
	private:
		eSoundState _soundState;

	};
}