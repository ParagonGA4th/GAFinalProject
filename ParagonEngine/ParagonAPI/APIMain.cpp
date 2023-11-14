#include "APIMain.h"
#include "PgInput.h"
#include "PgTime.h"
#include <singleton-cpp/singleton.h>
namespace Pg::API
{
	void APIMain::Initialize()
	{
		auto& tPgInput = singleton<Pg::API::Input::PgInput>();
		this->_pgInput = &tPgInput;

		auto& tPgTime = singleton<Pg::API::Time::PgTime>();
		this->_pgTime = &tPgTime;

		_pgInput->Initialize();
		_pgTime->Initialize();
	}

	void APIMain::Finalize()
	{

	}
}