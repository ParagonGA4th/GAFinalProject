#include "APIMain.h"
#include "PgInput.h"
#include "PgTime.h"
#include "PgAudio.h"
#include "PgTween.h"
#include "PgRayCast.h"
#include "PgGraphics.h"
#include "PgScene.h"
#include "PgRecast.h"

#include <singleton-cpp/singleton.h>

namespace Pg::API
{
	void APIMain::Initialize()
	{
		auto& tPgInput = singleton<Pg::API::Input::PgInput>();
		this->_pgInput = &tPgInput;

		auto& tPgTime = singleton<Pg::API::Time::PgTime>();
		this->_pgTime = &tPgTime;

		auto& tPgAudio = singleton<Pg::API::Sound::PgAudio>();
		this->_pgAudio = &tPgAudio;

		auto& tPgRayCast = singleton<Pg::API::Raycast::PgRayCast>();
		this->_pgRayCast = &tPgRayCast;

		auto& tPgTween = singleton<Pg::API::Tween::PgTween>();
		this->_pgTween = &tPgTween;

		auto& tPgGraphics = singleton<Pg::API::Graphics::PgGraphics>();
		this->_pgGraphics = &tPgGraphics;

		auto& tPgScene = singleton<Pg::API::PgScene>();
		this->_pgScene = &tPgScene;

		auto& tPgRecast = singleton<Pg::API::Recast::PgRecast>();
		this->_pgRecast = &tPgRecast;

		_pgInput->Initialize();
		_pgTime->Initialize();
		_pgRayCast->Initialize();
		_pgAudio->Initialize();
		_pgTween->Initialize();
		_pgGraphics->Initialize();
		_pgScene->Initialize();
		_pgRecast->Initialize();
	}

	void APIMain::UpdateLoop()
	{
		_pgScene->UpdateLoop();
	}

	void APIMain::Finalize()
	{

	}



}