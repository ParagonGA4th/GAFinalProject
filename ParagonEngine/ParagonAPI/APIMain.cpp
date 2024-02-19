#include "APIMain.h"
#include "PgInput.h"
#include "PgTime.h"
#include "PgRayCast.h"
#include "PgSound.h"
#include <singleton-cpp/singleton.h>
namespace Pg::API
{
	void APIMain::Initialize()
	{
		auto& tPgInput = singleton<Pg::API::Input::PgInput>();
		this->_pgInput = &tPgInput;

		auto& tPgTime = singleton<Pg::API::Time::PgTime>();
		this->_pgTime = &tPgTime;

		auto& tPgRayCast = singleton<Pg::API::Raycast::PgRayCast>();
		this->_pgRayCast = &tPgRayCast;

		auto& tPgSound = singleton<Pg::API::Sound::PgSound>();
		this->_pgSound = &tPgSound;

		_pgInput->Initialize();
		_pgTime->Initialize();
		_pgRayCast->Initialize();
		_pgSound->Initialize();
	}

	void APIMain::Finalize()
	{

	}
}