#include "CSVHelper.h"
#include "rapidcsv.h"

#include <cassert>
#include <algorithm>
#include <regex>

namespace Pg::Util::Helper
{
	std::vector<std::string> CSVHelper::ReturnFilePathFromResourceCSV(const std::string& csvPath)
	{
		rapidcsv::Document doc(csvPath);

		auto& tPureData = doc.GetPureData();
		assert(tPureData.size() >= 1 && "�� �׷��� �⺻���� ���ҽ� ���˿� ��ġ X");
		std::vector<std::string> tRet;

		bool tPassedFirst = false;
		for (auto& it : tPureData)
		{
			if (!tPassedFirst)
			{
				tPassedFirst = true;
				continue;
			}

			//�� String Path�� ������ CSV Error ��������� ���鼭 ������.
			//assert(!() && "Empty String Detected - CSV ERROR!");
			if (it.at(0).empty())
			{
				continue;
			}

			//������ �� ����.
			tRet.push_back(it.at(0));
		}

		return tRet;
	}

	std::vector<std::string> CSVHelper::ReturnFilePathFromTexture2dCSV(const std::string& csvPath)
	{
		rapidcsv::Document doc(csvPath);

		auto& tPureData = doc.GetPureData();
		assert(tPureData.size() >= 1 && "�� �׷��� �⺻���� ���ҽ� ���˿� ��ġ X");

		std::vector<std::string> tRet;

		bool tPassedFirst = false;
		for (auto& it : tPureData)
		{
			if (!tPassedFirst)
			{
				tPassedFirst = true;
				continue;
			}

			//�� String Path�� ������ CSV Error ��������� ���鼭 ������.
			//assert(!() && "Empty String Detected - CSV ERROR!");
			if (it.at(0).empty())
			{
				continue;
			}

			//������ �� ����.
			std::string tInputTexturePath = it.at(0);
			//std::string tIgColorSpaceRaw = it.at(1);
			//bool tIn = false;
			//if ((tIgColorSpaceRaw.compare("NULL") != 0) && (!tIgColorSpaceRaw.empty()))
			//{
			//	//NULL�̸� ����.
			//	int tBoolInt = std::stoi(tIgColorSpaceRaw);
			//	assert((tBoolInt == 0 || tBoolInt == 1) && "0/1�� �ƴϸ� Bool ġȯ �Ұ���.");
			//	tIn = static_cast<bool>(tBoolInt);
			//}
			//
			//tInputTexturePath += "#";
			//tInputTexturePath.append(std::to_string(tIn));
			tRet.push_back(tInputTexturePath);
		}

		return tRet;
	}

	std::vector<std::tuple<std::string, Pg::Data::eSoundGroup, bool>> CSVHelper::ReturnFilePathFromSoundFileCSV(const std::string& csvPath)
	{
		rapidcsv::Document doc(csvPath);

		auto& tPureData = doc.GetPureData();
		assert(tPureData.size() >= 1 && "�� �׷��� �⺻���� ���ҽ� ���˿� ��ġ X");

		std::vector<std::tuple<std::string, Pg::Data::eSoundGroup, bool>> tRet;
		//tRet.resize(tPureData.size() - 1);

		for (int i = 0; i < tPureData.size() - 1; i++)
		{
			//ó���� �ִ� �ȳ��� Text ��ŵ.
			auto& it = tPureData.at(i + 1);

			////�Űܴ��� ����.
			//tRet.push_back(std::make_tuple("", Pg::Data::eSoundGroup::Count, false));
			//assert(!(it.at(0).empty()) && "Empty String Detected - CSV ERROR!");

			if (it.at(0).empty())
			{
				continue;
			}

			//�� ��� ����ֱ�.
			tRet.push_back({});

			//Path String �Űܴ��.
			std::get<0>(tRet.at(i)) = it.at(0);

			//SoundGroup ����.
			std::string tEnumGroupStr = it.at(1);
			Pg::Data::eSoundGroup tEnumSound = Pg::Data::eSoundGroup::Count; //����.

			//Ȥ�ó� ���� ���� �ִ� ���� ����.
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
				assert(false && "ȣȯ���� �ʴ� Enum/String Format");
			}
			std::get<1>(tRet.at(i)) = tEnumSound;

			//���� isLooping ������Ƽ�� ���� ����.
			int tLoopInt = std::stoi(it.at(2));
			assert((tLoopInt == 0 || tLoopInt == 1) && "0/1�� �ƴϸ� Bool ġȯ �Ұ���.");
			std::get<2>(tRet.at(i)) = static_cast<bool>(tLoopInt);
		}

		return tRet;
	}

	std::vector<std::pair<std::string, bool>> CSVHelper::ReturnFilePathFromBTreeCSV(const std::string& csvPath)
	{
		rapidcsv::Document doc(csvPath);

		auto& tPureData = doc.GetPureData();
		assert(tPureData.size() >= 1 && "�� �׷��� �⺻���� ���ҽ� ���˿� ��ġ X");

		std::vector<std::pair<std::string, bool>> tRet;
		//tRet.resize(tPureData.size() - 1);

		for (int i = 0; i < tPureData.size() - 1; i++)
		{
			//ó���� �ִ� �ȳ��� Text ��ŵ.
			auto& it = tPureData.at(i + 1);

			//assert(!(it.at(0).empty()) && "Empty String Detected - CSV ERROR!");
			if (it.at(0).empty())
			{
				continue;
			}

			tRet.push_back({});

			tRet.at(i).first = it.at(0);

			int tBoolInt = std::stoi(it.at(1));
			assert((tBoolInt == 0 || tBoolInt == 1) && "0/1�� �ƴϸ� Bool ġȯ �Ұ���.");
			tRet.at(i).second = static_cast<bool>(tBoolInt);
		}

		return tRet;
	}

	void CSVHelper::TurnDebugInPathToRelease(std::vector<std::string>& originalCSVVec)
	{
		for (int i = 0; i < originalCSVVec.size(); i++)
		{
			originalCSVVec.at(i) = std::regex_replace(originalCSVVec.at(i), std::regex("Debug"), "Release"); // ��ü: 'Debug' -> 'Release'
		}

		return;
	}

	std::vector<std::pair<std::string, std::string>> CSVHelper::ReturnFilePathFromLightmapCSV(const std::string& csvPath)
	{
		rapidcsv::Document doc(csvPath);

		auto& tPureData = doc.GetPureData();
		assert(tPureData.size() >= 1 && "�� �׷��� �⺻���� ���ҽ� ���˿� ��ġ X");

		std::vector<std::pair<std::string, std::string>> tRet;

		for (int i = 0; i < tPureData.size() - 1; i++)
		{
			//ó���� �ִ� �ȳ��� Text ��ŵ.
			auto& it = tPureData.at(i + 1);

			//assert(!(it.at(0).empty()) && "Empty String Detected - CSV ERROR!");
			if (it.at(0).empty())
			{
				continue;
			}

			tRet.push_back({});
			tRet.at(i).first = it.at(0);
			tRet.at(i).second = it.at(1);
		}

		return tRet;
	}

	std::vector<Pg::Data::VisualEffectData> CSVHelper::ReturnDataFromVisualEffectCSV(const std::string& csvPath)
	{
		rapidcsv::Document doc(csvPath);

		auto& tPureData = doc.GetPureData();
		assert(tPureData.size() >= 1 && "�� �׷��� �⺻���� ���ҽ� ���˿� ��ġ X");

		std::vector<Pg::Data::VisualEffectData> tRet;

		for (int i = 0; i < tPureData.size() - 1; i++)
		{
			//ó���� �ִ� �ȳ��� Text ��ŵ.
			auto& it = tPureData.at(i + 1);
			if (it.at(0).empty())
			{
				continue;
			}

			//���� �Űܴ�´� �����.
			tRet.push_back({});
			auto& tInput = tRet.at(i);

			//EffectName
			{
				tInput._effectName = it.at(0);
			}

			//Is3DSpace
			{
				int tBoolInt = std::stoi(it.at(1));
				assert((tBoolInt == 0 || tBoolInt == 1) && "0/1�� �ƴϸ� Bool ġȯ �Ұ���.");
				tInput._is3dSpace = static_cast<bool>(tBoolInt);
			}

			//IsFaceCamera
			{
				int tBoolInt = std::stoi(it.at(2));
				assert((tBoolInt == 0 || tBoolInt == 1) && "0/1�� �ƴϸ� Bool ġȯ �Ұ���.");
				tInput._isFaceCamera = static_cast<bool>(tBoolInt);
			}

			//IsAlphaBlended
			{
				int tBoolInt = std::stoi(it.at(3));
				assert((tBoolInt == 0 || tBoolInt == 1) && "0/1�� �ƴϸ� Bool ġȯ �Ұ���.");
				tInput._isAlphaBlended = static_cast<bool>(tBoolInt);
			}

			//AlphaPercentage
			{
				float tAlphaPer = std::stof(it.at(4));
				tInput._alphaPercentage = std::clamp<float>(tAlphaPer, 0.f, 100.f);
			}

			//TextureName (���� dds �� �⺻ �ؽ��� �̸�, �̹� �ٸ� Texture CSV���� �ε�Ǿ����.)
			{
				tInput._textureName = it.at(5);
			}
			
			//eSpriteMode
			{
				int tInt = std::stoi(it.at(6));
				tInput._spriteMode = static_cast<Pg::Data::eSpriteMode>(tInt);
			}

			//���� IsSpriteSheet�� FrameCount ����� �˾ƾ� ��.
			if (tInput._spriteMode == Pg::Data::eSpriteMode::_SPRITE_SHEET)
			{
				tInput._frameCount = std::stoul(it.at(7));
			}

			//IsUseCustomVertexShader
			{
				int tBoolInt = std::stoi(it.at(8));
				assert((tBoolInt == 0 || tBoolInt == 1) && "0/1�� �ƴϸ� Bool ġȯ �Ұ���.");
				tInput._isUseCustomVertexShader = static_cast<bool>(tBoolInt);
			}

			//���� Custom�� ����Ѵٸ�, VertexShader �̸� �Ű� ���.
			if (tInput._isUseCustomVertexShader)
			{
				tInput._customVertexShaderName = it.at(9);
			}

			//IsUseCustomPixelShader
			{
				int tBoolInt = std::stoi(it.at(10));
				assert((tBoolInt == 0 || tBoolInt == 1) && "0/1�� �ƴϸ� Bool ġȯ �Ұ���.");
				tInput._isUseCustomPixelShader = static_cast<bool>(tBoolInt);
			}

			//���� Custom ����, PixelShader �̸� �Űܴ��.
			if (tInput._isUseCustomPixelShader)
			{
				tInput._customPixelShaderName = std::stoi(it.at(11));
			}

			//ManualSwitchingMode
			{
				int tBoolInt = std::stoi(it.at(12));
				assert((tBoolInt == 0 || tBoolInt == 1) && "0/1�� �ƴϸ� Bool ġȯ �Ұ���.");
				tInput._manualSwitchingMode = static_cast<bool>(tBoolInt);
			}

			//IgnoreDepth 
			{
				int tBoolInt = std::stoi(it.at(13));
				assert((tBoolInt == 0 || tBoolInt == 1) && "0/1�� �ƴϸ� Bool ġȯ �Ұ���.");
				tInput._ignoreDepth = static_cast<bool>(tBoolInt);
			}

			//IsQuadLyingDown
			{
				int tBoolInt = std::stoi(it.at(14));
				assert((tBoolInt == 0 || tBoolInt == 1) && "0/1�� �ƴϸ� Bool ġȯ �Ұ���.");
				tInput._isQuadLyingDown = static_cast<bool>(tBoolInt);
			}
		}

		return tRet;

	}
}