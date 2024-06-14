#include "InstancingException.h"
#include "../ParagonHelper/CSVHelper.h"

namespace Pg::Util
{
	InstancingException::InstancingException()
	{

	}

	InstancingException::~InstancingException()
	{

	}

	void InstancingException::Initialize(const std::string& resourceListPath)
	{
		std::string tPath = resourceListPath + "/14_InstancingExceptionModels.csv";
		auto tVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
		for (auto& it : tVec)
		{
			INSTANCING_EXCEPTION_MESH_PATH_SET.insert(it);
		}
	}

	bool InstancingException::IsExceptionFromInstance(const std::string& meshPath)
	{
		return INSTANCING_EXCEPTION_MESH_PATH_SET.contains(meshPath);
	}



}