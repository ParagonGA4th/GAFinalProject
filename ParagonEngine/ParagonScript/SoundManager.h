#pragma once
#include "ScriptInterface.h"
/// <summary>
/// ���� ���ο� ���� ���� �Ŵ���.
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


