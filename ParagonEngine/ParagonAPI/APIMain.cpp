#include "APIMain.h"
#include "PgInput.h"
#include "PhysXTest.h"
#include <singleton-cpp/singleton.h>
namespace Pg::API
{
	void APIMain::Initialize()
	{
		auto& tPgInput = singleton<Pg::API::Input::PgInput>();
		this->_pgInput = &tPgInput;
		_pgInput->Initialize();

		//PhysX
		//_test = new PhysXTest();
		//_test->Initialize();
	}

	void APIMain::Finalize()
	{

	}
}