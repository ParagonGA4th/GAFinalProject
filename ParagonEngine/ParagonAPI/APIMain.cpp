#include "APIMain.h"
#include "PgInput.h"
#include "PgTime.h"
#include "PgAudio.h"
#include "PgRayCast.h"
#include <singleton-cpp/singleton.h>
namespace Pg::API
{
	void APIMain::Initialize()
	{
		auto& tPgInput = singleton<Pg::API::Input::PgInput>();
		this->_pgInput = &tPgInput;

		auto& tPgTime = singleton<Pg::API::Time::PgTime>();
		this->_pgTime = &tPgTime;

		//auto& tPgSound = singleton<Pg::API::Sound::PgSound>();
		//this->_pgSound = &tPgSound;

		auto& tPgAudio = singleton<Pg::API::Sound::PgAudio>();
		this->_pgAudio = &tPgAudio;

		auto& tPgRayCast = singleton<Pg::API::Raycast::PgRayCast>();
		this->_pgRayCast = &tPgRayCast;

		_pgInput->Initialize();
		_pgTime->Initialize();
		_pgRayCast->Initialize();
		_pgAudio->Initialize();
	}

	void APIMain::Finalize()
	{

	}
}