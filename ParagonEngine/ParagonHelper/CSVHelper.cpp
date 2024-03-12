#include "CSVHelper.h"
#include "rapidcsv.h"

#include <cassert>

namespace Pg::Util::Helper
{
	std::vector<std::string> CSVHelper::ReturnFilePathFromResourceCSV(const std::string& csvPath)
	{
		rapidcsv::Document doc(csvPath);

		auto& tPureData = doc.GetPureData();
		assert(tPureData.size() >= 1 && "안 그러면 기본적인 리소스 포맷에 일치 X");
		std::vector<std::string> tRet;

		bool tPassedFirst = false;
		for (auto& it : tPureData)
		{
			if (!tPassedFirst)
			{
				tPassedFirst = true;
				continue;
			}

			//빈 String Path가 있으면 CSV Error 명시적으로 내면서 뻑내기.
			assert(!(it.at(0).empty()) && "Empty String Detected - CSV ERROR!");

			//실제로 값 투입.
			tRet.push_back(it.at(0));
		}
		
		return tRet;
	}

	std::vector<std::tuple<std::string, Pg::Data::eSoundGroup, bool>> CSVHelper::ReturnFilePathFromSoundFileCSV(const std::string& csvPath)
	{
		rapidcsv::Document doc(csvPath);

		auto& tPureData = doc.GetPureData();
		assert(tPureData.size() >= 1 && "안 그러면 기본적인 리소스 포맷에 일치 X");

		std::vector<std::tuple<std::string, Pg::Data::eSoundGroup, bool>> tRet;
		tRet.resize(tPureData.size() - 1);

		for (int i = 0; i < tPureData.size()-1; i++)
		{
			//처음에 있는 안내용 Text 스킵.
			auto& it = tPureData.at(i+1);

			////옮겨담을 원소.
			//tRet.push_back(std::make_tuple("", Pg::Data::eSoundGroup::Count, false));
			assert(!(it.at(0).empty()) && "Empty String Detected - CSV ERROR!");

			//Path String 옮겨담기.
			std::get<0>(tRet.at(i)) = it.at(0);
			
			//SoundGroup 이전.
			std::string tEnumGroupStr = it.at(1);
			Pg::Data::eSoundGroup tEnumSound = Pg::Data::eSoundGroup::Count; //리셋.

			//혹시나 있을 수도 있는 공백 제거.
			tEnumGroupStr.erase(std::remove(tEnumGroupStr.begin(), tEnumGroupStr.end(), ' '), tEnumGroupStr.end());
			if (tEnumGroupStr.compare("BGM") == 0)
			{
				tEnumSound = Pg::Data::eSoundGroup::BGM;
			}
			else if (tEnumGroupStr.compare("InGame") == 0)
			{
				tEnumSound = Pg::Data::eSoundGroup::InGame;
			}
			else if (tEnumGroupStr.compare("Effect") == 0)
			{
				tEnumSound = Pg::Data::eSoundGroup::Effect;
			}
			else
			{
				assert(false && "호환되지 않는 Enum/String Format");
			}
			std::get<1>(tRet.at(i)) = tEnumSound;

			//이제 isLooping 프로퍼티를 받을 차례.
			int tLoopInt = std::stoi(it.at(2));
			assert((tLoopInt == 0 || tLoopInt == 1) && "0/1이 아니면 Bool 치환 불가능.");
			std::get<2>(tRet.at(i)) = static_cast<bool>(tLoopInt);
		}

		return tRet;
	}

	std::vector<std::pair<std::string,bool>> CSVHelper::ReturnFilePathFromBTreeCSV(const std::string& csvPath)
	{
		rapidcsv::Document doc(csvPath);

		auto& tPureData = doc.GetPureData();
		assert(tPureData.size() >= 1 && "안 그러면 기본적인 리소스 포맷에 일치 X");

		std::vector<std::pair<std::string, bool>> tRet;
		tRet.resize(tPureData.size() - 1);

		for (int i = 0; i < tPureData.size() - 1; i++)
		{
			//처음에 있는 안내용 Text 스킵.
			auto& it = tPureData.at(i + 1);

			assert(!(it.at(0).empty()) && "Empty String Detected - CSV ERROR!");

			tRet.at(i).first = it.at(0);

			int tBoolInt = std::stoi(it.at(1));
			assert((tBoolInt == 0 || tBoolInt == 1) && "0/1이 아니면 Bool 치환 불가능.");
			tRet.at(i).second = static_cast<bool>(tBoolInt);
		}

		return tRet;
	}

}