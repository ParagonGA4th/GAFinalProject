#pragma once
#include <string>
#include <vector>

namespace Pg::Util::Helper
{
	class CSVHelper
	{
	public:
		static std::vector<std::string> ReturnFilePathFromResourceCSV(const std::string& csvPath);
	};
}