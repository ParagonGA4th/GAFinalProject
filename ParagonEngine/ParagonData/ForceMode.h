#pragma once

namespace Pg::Data
{
	//힘을 가하는 방법
	enum class ForceMode : int
	{
		eFORCE,
		eIMPULSE,
		eACCELERATION,
		eVELOCITY_CHANGE
	};
}