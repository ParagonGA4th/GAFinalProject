#include "CSVHelper.h"
#include "rapidcsv.h"

namespace Pg::Util::Helper
{
	std::vector<std::string> CSVHelper::ReturnFilePathFromResourceCSV(const std::string& csvPath)
	{
		rapidcsv::Document doc(csvPath);

		auto& tPureData = doc.GetPureData();
		
		std::vector<std::string> tRet;

		bool tPassedFirst = false;
		for (auto& it : tPureData)
		{
			if (!tPassedFirst)
			{
				tPassedFirst = true;
				continue;
			}
			tRet.push_back(it.at(0));
		}
		
		return tRet;
	}

}