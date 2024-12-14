#pragma once
#include <set>
#include <string>
#include "UtilDLLExporter.h"

/// <summary>
/// Instancing Exception List 별도로 관리.
/// </summary>

namespace Pg::Util
{
	class InstancingException
	{
	public:
		PARAGON_UTIL_DLL InstancingException();
		PARAGON_UTIL_DLL ~InstancingException();
		PARAGON_UTIL_DLL void Initialize(const std::string& resourceListPath);
		PARAGON_UTIL_DLL bool IsExceptionFromInstance(const std::string& meshPath);

	private:
		std::set<std::string> INSTANCING_EXCEPTION_MESH_PATH_SET;
	};
}


