#include "UtilMain.h"
#include "Log.h"
#include <singleton-cpp/singleton.h>

namespace Pg::Util
{
	void UtilMain::Initialize()
	{
		auto& _logger = singleton<Pg::Util::Debug::Log>();
		_logger.Initialize();
		_logger.SetLoggerLevel(0);
	}

	void UtilMain::Finalize()
	{

	}
}