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

	void AssetManager::InitializeDefaults()
	{
		_perFrameToLoadResources.reserve(30);
		_perFrameToUnloadResources.reserve(30);

		//����Ʈ ���ҽ����� �ε�Ǵ� �Լ�. �ʼ�!
		LoadDefaultResources();
	}

	void AssetManager::Initialize(Pg::Core::ProcessMain* core, const std::string& resourceListPath) 
	{
		_coreMain = core;
		//Scene ���� ���ҽ� ������ �Ϸ���� ������, ���⼭ �ε���� ���� ���ҽ����� ������ ����.

		LoadResourcesFromCSV(resourceListPath);
	}

	void AssetManager::Update(Pg::Core::IEngine* engine, Pg::Core::IGraphics* graphics)
	{
		//���� Loading ����.
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
			
			//������ �ε�Ǿ�����, AssetManager�� ��Ͽ� �����ؼ� �����ؾ� �Ѵ�.
			this->_resourceMap.insert(std::make_pair(it.first, it.second));
		}

		//���� Unloading ����.
		for (auto& it : _perFrameToUnloadResources)
		{
			graphics->UnloadResource(it);
			engine->UnloadResource(it);

			//������ ��ε�Ǿ�����, AssetManager�� ��Ͽ� �����ؼ� �����ؾ� �Ѵ�.
			this->_resourceMap.erase(it);
		}

		//���� Iteration�� ���� Clear.
		_perFrameToLoadResources.clear();
		_perFrameToUnloadResources.clear();

		//2�� �߻� ���ҽ����� Graphics���� ������ �ʾҴ� üũ���ش�.
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

		//�ߺ� ��Ұ� �ִ��� Ȯ��.
		bool tWasUnique = (it == _pathNamesVector.end());


		PG_WARN("Main �귣ġ�� ��ģ ��, �ش� ���� ���ҽ� �ߺ� �˻� ���� Ȱ��ȭ�ؾ�.");
		//if (!tWasUnique)
		//{
		//	//assert false �ϱ� ����, ���� �ڵ� �� ���.
		//	std::set<std::string> _tempDupNameResourceSet;
		//
		//	//���������� �ߺ����� ���� Iterator
		//	auto tRend = std::make_reverse_iterator(std::prev(it));
		//	for (auto endIt = _pathNamesVector.rbegin(); endIt != tRend; endIt++)
		//	{
		//		_tempDupNameResourceSet.insert(*endIt);
		//	}
		//
		//	//std::unique ��ü�� ����� "" ����. (ũ�⸦ �ٲ��� �ʱ� ������)
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
		//	assert(false && "���⼭ �ɸ��� �ߺ��Ǵ� ���� �̸��� ���� �ٸ� ����� ���ҽ����� �ִٴ� ���̴�. �����ؾ�.");
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

		//Load�ϱ� ����, �̹� ��Ͽ� ������ üũ!
		if (!IsExistResource(tFilePath))
		{
			auto tInfo = std::make_pair(tFilePath, define);
			this->_perFrameToLoadResources.emplace_back(tInfo);
		}
	}

	void AssetManager::UnloadResource(const std::string& filepath)
	{
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filepath);

		//Unload�ϱ� ����, �̹� ��Ͽ� �ִ��� üũ!
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

		//���� ���� ���, ����Ʈ�� ����� �߰��Ѵ�.
		for (auto& it : *tSecondaryResources)
		{
			//������ Ű�� ���� ��츸 �־��ش�.
			_resourceMap.insert(std::make_pair(it.first, it.second));
		}

		//�׷��Ƚ� Secondary List Ŭ����.
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
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromTexture2dCSV(tPath);
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

#if defined(NDEBUG) | defined(_NDEBUG)
			Pg::Util::Helper::CSVHelper::TurnDebugInPathToRelease(tPathVec);
#endif

			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_RENDER_VERTEXSHADER);
			}
		}

		//RenderPixelShader
		{
			std::string tPath = tUniformPath + "/7_RenderPixelShader.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);

#if defined(NDEBUG) | defined(_NDEBUG)
			Pg::Util::Helper::CSVHelper::TurnDebugInPathToRelease(tPathVec);
#endif

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

		//Sounds / BehaviorTree�� ���� �۵��� ��! ����� �ڵ带 ����ϱ�� �ϰ�����.

		//RenderAnimation
		{
			std::string tPath = tUniformPath + "/12_Animation.csv";
			auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromResourceCSV(tPath);
			for (auto& it : tPathVec)
			{
				LoadResource(it, Pg::Data::Enums::eAssetDefine::_ANIMATION);
			}
		}

		//Lightmap XML�� �������� Graphics���� �����Ѵ�.
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

		LoadResource(Pg::Defines::ASSET_DEFAULT_PRELOADED_LUT_PATH, eAssetDefine::_TEXTURE2D);

		//�Ϲ� Cubemap
		LoadResource(Pg::Defines::ASSET_DEFAULT_ENVIRONMENT_CUBEMAP_PATH, eAssetDefine::_CUBEMAP);

		//IBL�� Ȱ��.
		LoadResource(Pg::Defines::ASSET_DEFAULT_IBL_DIFFUSE_IRRADIANCE_CUBEMAP_PATH, eAssetDefine::_CUBEMAP);
		LoadResource(Pg::Defines::ASSET_DEFAULT_IBL_SPECULAR_IRRADIANCE_CUBEMAP_PATH, eAssetDefine::_CUBEMAP);
		LoadResource(Pg::Defines::ASSET_DEFAULT_IBL_SPECULAR_BRDF_LUT_TEXTURE_PATH, eAssetDefine::_TEXTURE2D);

		using Pg::Util::Helper::ResourceHelper;

		LoadResource(ResourceHelper::IfReleaseChangeDebugText(Pg::Defines::DEFAULT_APPENDS_RENDER_VS_PATH), eAssetDefine::_RENDER_VERTEXSHADER);
		LoadResource(ResourceHelper::IfReleaseChangeDebugText(Pg::Defines::DEFAULT_APPENDS_RENDER_PS_PATH), eAssetDefine::_RENDER_PIXELSHADER);

		LoadResource(ResourceHelper::IfReleaseChangeDebugText(Pg::Defines::DEFAULT_ALPHA_STATIC_VS_PATH), eAssetDefine::_RENDER_VERTEXSHADER);
		LoadResource(ResourceHelper::IfReleaseChangeDebugText(Pg::Defines::DEFAULT_ALPHA_SKINNED_VS_PATH), eAssetDefine::_RENDER_VERTEXSHADER);
		LoadResource(ResourceHelper::IfReleaseChangeDebugText(Pg::Defines::DEFAULT_ALPHA_PS_PATH), eAssetDefine::_RENDER_PIXELSHADER);
	}


	std::vector<std::string> AssetManager::GetResourcesPathByDefine(Pg::Data::Enums::eAssetDefine define)
	{
		std::vector<std::string> tRet;

		for (const auto& [path, saveDefine] : _resourceMap)
		{
			if (saveDefine == define)
			{
				tRet.push_back(path);
			}
		}

		return tRet;
	}

	std::vector<std::string> AssetManager::GetResourcesNameByDefine(Pg::Data::Enums::eAssetDefine define)
	{
		std::vector<std::string> tRet;

		for (const auto& [path, saveDefine] : _resourceMap)
		{
			if (saveDefine == define)
			{
				tRet.push_back(path);
			}
		}

		for (auto& it : tRet)
		{
			it = ResourceHelper::GetNameFromPath(it);
		}

		return tRet;
	}

	std::vector<std::string> AssetManager::GetMaterialNamesWithoutDefault()
	{
		std::vector<std::string> tRet;

		for (const auto& [path, saveDefine] : _resourceMap)
		{
			if (saveDefine == Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL &&
				path.find(Pg::Defines::GENERATED_MATERIAL_PREFIX) == std::string::npos)
			{
				//����Ʈ �����Ƚ��� ��ã���� �� (== Custom Material)
				tRet.push_back(path);
			}
		}

		for (auto& it : tRet)
		{
			it = ResourceHelper::GetNameFromPath(it);
		}

		return tRet;
	}

	

}
