#include "CSVHelper.h"
#include "rapidcsv.h"

namespace Pg::Util::Helper
{
	std::vector<std::string> CSVHelper::ReturnFilePathFromResourceCSV(const std::string& csvPath)
	{
		rapidcsv::Document doc(csvPath);

		auto& tPureData = doc.GetPureData();

		
		return {};
	}

}