#pragma once
#include <foundation/PxErrorCallback.h>
#include <string>
#include <filesystem>
#include "../ParagonUtil/Log.h"

//PhysX�� �����ϴ� Error Code �ޱ� ����.
//Flax ����.

namespace Pg::Engine
{
	class PhysicsErrorCallback : public physx::PxErrorCallback
	{
		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override
		{
			//Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>();
			//if (tLogger.IsInitialized())
			//{
			//	//File Name ONLY.
			//	std::filesystem::path tFile = file;
			//
			//	PG_WARN("PhysX Error! Code: {0}.\n{1}\nSource: {2} : {3}.", static_cast<int>(code), std::string(message), tFile.filename().string(), line);
			//}
		}
	};
}





