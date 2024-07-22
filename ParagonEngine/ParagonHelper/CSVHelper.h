#pragma once
#include "../ParagonData/eSoundGroup.h"
#include "../ParagonData/VisualEffectData.h"

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
		
		 //Texture2DPath + append IgnoreSRGB
		 static std::vector<std::string> ReturnFilePathFromTexture2dCSV(const std::string& csvPath);

		 //Sound를 위한 CSV Helper.
		 static std::vector<std::tuple<std::string, Pg::Data::eSoundGroup, bool>> ReturnFilePathFromSoundFileCSV(const std::string& csvPath);

		//Behavior Tree를 위한 CSV Helper. bool = isUniform.
		 static std::vector<std::pair<std::string, bool>> ReturnFilePathFromBTreeCSV(const std::string& csvPath);

		 static std::vector<std::pair<std::string, std::string>> ReturnFilePathFromLightmapCSV(const std::string& csvPath);

		 static void TurnDebugInPathToRelease(std::vector<std::string>& originalCSVVec);
		 
		 //EffectName,Is3DSpace,IsFaceCamera,AlphaBlended,AlphaPercentage,TextureName,IsGIF
		 static std::vector<Pg::Data::VisualEffectData> ReturnDataFromVisualEffectCSV(const std::string& csvPath);
	};
}