#pragma once
#include "ScriptInterface.h"
/// <summary>
/// 게임 내부에 쓰일 사운드 매니저.
/// </summary>

namespace Pg::Data
{
	class AudioSource;
}

namespace Pg::DataScript
{
	class SoundManager : public ScriptInterface<SoundManager>
	{
		DEFINE_PARAGON_SCRIPT(SoundManager);
	public:
		SoundManager(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::Data::AudioSource* _ingameBgm;
	};
}


