#pragma once
#include "../ParagonData/eSoundGroup.h"


#include <string>
#include <vector>
#include <tuple>

namespace Pg::Util::Helper
{
	class CSVHelper
	{
	public:
		//Sound / BT를 제외한 모든 리소스에 활용.
		 static std::vector<std::string> ReturnFilePathFromResourceCSV(const std::string& csvPath);
		
		//Sound를 위한 CSV Helper.
		 static std::vector<std::tuple<std::string, Pg::Data::eSoundGroup, bool>> ReturnFilePathFromSoundFileCSV(const std::string& csvPath);

		//Behavior Tree를 위한 CSV Helper. bool = isUniform.
		 static std::vector<std::pair<std::string, bool>> ReturnFilePathFromBTreeCSV(const std::string& csvPath);
	};
}