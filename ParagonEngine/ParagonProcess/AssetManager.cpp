#include "AssetManager.h"
#include "ProcessMain.h"
#include "IEngine.h"
#include "IGraphics.h"

#include "../ParagonData/ParagonDefines.h"
#include "../ParagonGraphics/GraphicsResourceManager.h"
#include "../ParagonGameEngine/EngineResourceManager.h"

#include "../ParagonHelper/ResourceHelper.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonHelper/CSVHelper.h"

//<ResourcesList>
#include "../ParagonGraphics/RenderMaterial.h"
//</ResourcesList>

#include <algorithm>
#include <filesystem>
#include <cassert>
#include <set>
#include <iterator>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonData.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonData.lib")
#endif // _DEBUG

namespace Pg::Core::Manager
{
	using Pg::Util::Helper::ResourceHelper;

	AssetManager::AssetManager()
	{
		//
	}

	AssetManager::~AssetManager()
	{

	}


	void AssetManager::Initialize(Pg::Core::ProcessMain* core, const std::string& resourceListPath) 
	{
		_coreMain = core;
		_perFrameToLoadResources.reserve(30);
		_perFrameToUnloadResources.reserve(30);

		//디폴트 리소스들이 로드되는 함수. 필수!
		LoadDefaultResources();

		//Scene 단위 리소스 연동이 완료되지 않으면, 여기서 로드되지 않은 리소스들은 사용되지 못함.

		LoadResourcesFromCSV(resourceListPath);
	}

	void AssetManager::Update(Pg::Core::IEngine* engine, Pg::Core::IGraphics* graphics)
	{
		//실제 Loading 로직.
		for (auto& it : _perFrameToLoadResources)
		{
			if (Pg::Util::Helper::ResourceHelper::IsGraphicsResource(it.second))
			{
				graphics->LoadResource(it.first, it.second);
			}
			else
			{
				engine->LoadResource(it.first, it.second);
			}
			
			//실제로 로드되었으면, AssetManager의 목록에 연동해서 관리해야 한다.
			this->_resourceMap.insert(std::make_pair(it.first, it.second));
		}

		//실제 Unloading 로직.
		for (auto& it : _perFrameToUnloadResources)
		{
			graphics->UnloadResource(it);
			engine->UnloadResource(it);

			//실제로 언로드되었으면, AssetManager의 목록에 연동해서 관리해야 한다.
			this->_resourceMap.erase(it);
		}

		//다음 Iteration을 위해 Clear.
		_perFrameToLoadResources.clear();
		_perFrameToUnloadResources.clear();

		//2차 발생 리소스들이 Graphics에서 나오지 않았다 체크해준다.
		CheckForGraphicsToProcessLoad(graphics);
	}

	void AssetManager::AssureNoNameDuplicates()
	{
		std::vector<std::string> _pathNamesVector;
		_pathNamesVector.reserve(_resourceMap.size());

		//std::set
		for (auto& [bPathName, bAssetDefine] : _resourceMap)
		{
			std::filesystem::path tPath = bPathName;
			std::string tName = tPath.filename().generic_string();
			assert(!tName.empty());

			_pathNamesVector.push_back(tName);
		}
		std::sort(_pathNamesVector.begin(), _pathNamesVector.end());
		std::vector<std::string>::iterator it = std::unique(_pathNamesVector.begin(), _pathNamesVector.end());

		//중복 요소가 있는지 확인.
		bool tWasUnique = (it == _pathNamesVector.end());


		PG_WARN("Main 브랜치와 합친 뒤, 해당 줄의 리소스 중복 검사 로직 활성화해야.");
		//if (!tWasUnique)
		//{
		//	//assert false 하기 전에, 오류 코드 등 출력.
		//	std::set<std::string> _tempDupNameResourceSet;
		//
		//	//마지막으로 중복되지 않은 Iterator
		//	auto tRend = std::make_reverse_iterator(std::prev(it));
		//	for (auto endIt = _pathNamesVector.rbegin(); endIt != tRend; endIt++)
		//	{
		//		_tempDupNameResourceSet.insert(*endIt);
		//	}
		//
		//	//std::unique 자체로 생기는 "" 제거. (크기를 바꾸지 않기 때문에)
		//	_tempDupNameResourceSet.erase("");
		//
		//	std::string tDebugLogString = "Duplicates : \n";
		//	for (auto& itt : _tempDupNameResourceSet)
		//	{
		//		tDebugLogString += " < ";
		//		tDebugLogString += itt;
		//		tDebugLogString += " > ";
		//	}
		//
		//	tDebugLogString += "\n";
		//	tDebugLogString += "Total Repeated Count : ";
		//	tDebugLogString += std::to_string(_tempDupNameResourceSet.size());
		//
		//	PG_ERROR(tDebugLogString.c_str());
		//
		//	assert(false && "여기서 걸리면 중복되는 파일 이름을 가진 다른 경로의 리소스들이 있다는 것이다. 제거해야.");
		//}
	}

	bool AssetManager::IsExistResource(const std::string& filepath)
	{
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filepath);

		auto res = std::find_if(_resourceMap.begin(), _resourceMap.end(),
			[&tFilePath](const std::pair<std::string, Pg::Data::Enums::eAssetDefine>& val)
			-> bool {return (val.first == tFilePath); });

		bool tIsFound = (res != _resourceMap.end()) ? true : false;
		return tIsFound;
	}

	void AssetManager::LoadResource(const std::string& filepath, Pg::Data::Enums::eAssetDefine define)
	{
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filepath);

		//Load하기 전에, 이미 목록에 없는지 체크!
		if (!IsExistResource(tFilePath))
		{
			auto tInfo = std::make_pair(tFilePath, define);
			this->_perFrameToLoadResources.emplace_back(tInfo);
		}
	}

	void AssetManager::UnloadResource(const std::string& filepath)
	{
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filepath);

		//Unload하기 전에, 이미 목록에 있는지 체크!
		if (IsExistResource(tFilePath))
		{
			auto tInfo = tFilePath;
			this->_perFrameToUnloadResources.emplace_back(tInfo);
		}
	}

	void AssetManager::CheckForGraphicsToProcessLoad(Pg::Core::IGraphics* graphics)
	{
		auto tSecondaryResources = graphics->SendAddedSecondaryResources();

		if (tSecondaryResources == nullptr)
		{
			return;
		}

		//만약 있을 경우, 리스트에 목록을 추가한다.
		for (auto& it : *tSecondaryResources)
		{
			//기존에 키가 없을 경우만 넣어준다.
			_resourceMap.insert(std::make_pair(it.first, it.second));
		}

		//그래픽스 Secondary List 클리어.
		graphics->ClearSecondaryResourcesList();
	}

	void AssetManager::LoadResourcesFromCSV(const std::string& resourceListPath)
	{
		using Pg::Data::Enums::eAssetDefine;

		std::string tUniformPath = ResourceHelper::ForcePathUniformFull(resourceListPath);

		//Texture1D
		{
			std::string tPath = tUniformPath + "/0_Texture1D.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_TEXTURE1D);
			}
		}

		//Texture2D
		{
			std::string tPath = tUniformPath + "/1_Texture2D.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
			}
		}

		//Texture2DArray
		{
			std::string tPath = tUniformPath + "/2_Texture2DArray.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);
			}
		}

		//TextureCube
		{
			std::string tPath = tUniformPath + "/3_TextureCube.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_TEXTURECUBE);
			}
		}

		//Cubemap
		{
			std::string tPath = tUniformPath + "/4_Cubemap.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_CUBEMAP);
			}
		}

		//3DModel
		{
			std::string tPath = tUniformPath + "/5_3DModel.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_3DMODEL);
			}
		}

		//RenderVertexShader
		{
			std::string tPath = tUniformPath + "/6_RenderVertexShader.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_RENDER_VERTEXSHADER);
			}
		}

		//RenderPixelShader
		{
			std::string tPath = tUniformPath + "/7_RenderPixelShader.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_RENDER_PIXELSHADER);
			}
		}

		//RenderMaterial
		{
			std::string tPath = tUniformPath + "/8_RenderMaterial.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);
			}
		}

		//RenderFont
		{
			std::string tPath = tUniformPath + "/9_RenderFont.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_FONT);
			}
		}

		//Sounds / BehaviorTree는 따로 작동될 것! 비슷한 코드를 사용하기는 하겠지만.

		//RenderAnimation
		{
			std::string tPath = tUniformPath + "/12_Animation.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_ANIMATION);
			}
		}
	}

	void AssetManager::LoadDefaultResources()
	{
		using Pg::Data::Enums::eAssetDefine;

		LoadResource(Pg::Defines::ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_256, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_NORMAL_TEXTURE_PATH_256, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_ARM_TEXTURE_PATH_256, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_256, eAssetDefine::_TEXTURE2D);
		//LoadResource(Pg::Defines::ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_256, eAssetDefine::_TEXTURE2D);

		LoadResource(Pg::Defines::ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_512, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_NORMAL_TEXTURE_PATH_512, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_ARM_TEXTURE_PATH_512, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_512, eAssetDefine::_TEXTURE2D);
		//LoadResource(Pg::Defines::ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_512, eAssetDefine::_TEXTURE2D);

		LoadResource(Pg::Defines::ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_1024, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_NORMAL_TEXTURE_PATH_1024, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_ARM_TEXTURE_PATH_1024, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_1024, eAssetDefine::_TEXTURE2D);
		//LoadResource(Pg::Defines::ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_1024, eAssetDefine::_TEXTURE2D);

		LoadResource(Pg::Defines::ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_2048, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_NORMAL_TEXTURE_PATH_2048, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_ARM_TEXTURE_PATH_2048, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_2048, eAssetDefine::_TEXTURE2D);
		//LoadResource(Pg::Defines::ASSET_DEFAULT_DISPLACEMENT_TEXTURE_PATH_2048, eAssetDefine::_TEXTURE2D);

		//일반 Cubemap
		LoadResource(Pg::Defines::ASSET_DEFAULT_ENVIRONMENT_CUBEMAP_PATH, eAssetDefine::_CUBEMAP);

		//IBL에 활용.
		LoadResource(Pg::Defines::ASSET_DEFAULT_IBL_DIFFUSE_IRRADIANCE_CUBEMAP_PATH, eAssetDefine::_CUBEMAP);
		LoadResource(Pg::Defines::ASSET_DEFAULT_IBL_SPECULAR_IRRADIANCE_CUBEMAP_PATH, eAssetDefine::_CUBEMAP);
		LoadResource(Pg::Defines::ASSET_DEFAULT_IBL_SPECULAR_BRDF_LUT_TEXTURE_PATH, eAssetDefine::_TEXTURE2D);

		LoadResource(Pg::Defines::DEFAULT_APPENDS_RENDER_VS_PATH, eAssetDefine::_RENDER_VERTEXSHADER);
		LoadResource(Pg::Defines::DEFAULT_APPENDS_RENDER_PS_PATH, eAssetDefine::_RENDER_PIXELSHADER);
	}



}
