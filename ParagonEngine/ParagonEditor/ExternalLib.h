#pragma once
#include "../ParagonProcess/ProcessMain.h"
#include "../ParagonProcess/EditorAdapter.h"
#include "../ParagonAPI/PgInput.h"

#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonProcess.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonProcess.lib")
#endif // _DEBUG