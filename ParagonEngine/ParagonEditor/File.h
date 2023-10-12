#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace SuanTest
{
	namespace FileSys
	{
		class File
		{
		public:
			File(std::string filePath);

		public:
			void Load();


		private:
			std::string _filePath;

		};

	}
}


