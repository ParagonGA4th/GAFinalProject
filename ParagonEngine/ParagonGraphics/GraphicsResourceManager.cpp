#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"
#include "AssetBasic2DLoader.h"
#include "AssetBasic3DLoader.h"
#include "AssetCombinedLoader.h"

#include "RenderTexture2D.h" //디폴트 텍스쳐 로드 때문에.
#include "AssetTextureType.h"

#include "../ParagonData/ParagonDefines.h" 
#include "../ParagonHelper/ResourceHelper.h"

#include <cassert>
#include <filesystem>
#include <algorithm>

namespace Pg::Graphics::Manager
{
	GraphicsResourceManager::GraphicsResourceManager()
	{
		Pg::Graphics::Helper::GraphicsResourceHelper::Initialize();

		_asset2DLoader = std::make_unique<Pg::Graphics::Loader::AssetBasic2DLoader>();
		_asset3DLoader = std::make_unique<Pg::Graphics::Loader::AssetBasic3DLoader>();
		_assetCombinedLoader = std::make_unique<Pg::Graphics::Loader::AssetCombinedLoader>();
	}

	GraphicsResourceManager::~GraphicsResourceManager()
	{

	}

	Pg::Graphics::Loader::AssetBasic3DLoader* GraphicsResourceManager::GetBasic3DLoader()
	{
		assert(_asset3DLoader);
		return _asset3DLoader.get();
	}

	Pg::Graphics::Loader::AssetBasic2DLoader* GraphicsResourceManager::GetBasic2DLoader()
	{
		assert(_asset2DLoader);
		return _asset2DLoader.get();
	}

	Pg::Graphics::Loader::AssetCombinedLoader* GraphicsResourceManager::GetCombinedLoader()
	{
		assert(_assetCombinedLoader);
		return _assetCombinedLoader.get();
	}

	void GraphicsResourceManager::LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define)
	{
		//LoadResource 호출되었다는 것 = Asset이 아직 없다는 말.
		//Can Compile!
		//ASSETDEFINE2TYPE_MEMFUNC_2PARAMS(define, this, GraphicsResourceManager::CreateResource, filePath, define);
		
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filePath);

		using Pg::Data::Enums::eAssetDefine;

		if (define == eAssetDefine::_TEXTURE1D)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_TEXTURE1D)>(tFilePath, define);
		}
		else if (define == eAssetDefine::_TEXTURE2D)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_TEXTURE2D)>(tFilePath, define);
		}
		else if (define == eAssetDefine::_TEXTURE2DARRAY)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_TEXTURE2DARRAY)>(tFilePath, define);
		}
		else if (define == eAssetDefine::_TEXTURECUBE)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_TEXTURECUBE)>(tFilePath, define);
		}
		else if (define == eAssetDefine::_FONT)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_FONT)>(tFilePath, define);
		}
		else if (define == eAssetDefine::_CUBEMAP)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_CUBEMAP)>(tFilePath, define);
		}
		else if (define == eAssetDefine::_3DMODEL)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_3DMODEL)>(tFilePath, define);
		}
		else if (define == eAssetDefine::_RENDER_VERTEXSHADER)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_RENDER_VERTEXSHADER)>(tFilePath, define);
		}
		else if (define == eAssetDefine::_RENDER_PIXELSHADER)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_RENDER_PIXELSHADER)>(tFilePath, define);
		}
		else if (define == eAssetDefine::_RENDERMATERIAL)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_RENDERMATERIAL)>(tFilePath, define);
		}
		else if (define == eAssetDefine::_ANIMATION)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_ANIMATION)>(tFilePath, define);
		}
		else
		{
			assert(false); //여기까지 와도 안된다.
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_TEXTURE2D)>(tFilePath, define);
		}
	}

	void GraphicsResourceManager::UnloadResource(const std::string& filePath)
	{
		//Load와 달리, 동시에 두 개의 리소스 매니저가 동시에 호출된다. //지우지 못했어도 오류 반환하지 말자.


	}

	std::shared_ptr<Pg::Data::Resources::GraphicsResource> GraphicsResourceManager::GetResource(const std::string& path, Pg::Data::Enums::eAssetDefine define)
	{
		assert(!path.empty());

		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(path);
		using Pg::Data::Enums::eAssetDefine;

		if (define == eAssetDefine::_TEXTURE1D)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_TEXTURE1D)>(tFilePath);
		}
		else if (define == eAssetDefine::_TEXTURE2D)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_TEXTURE2D)>(tFilePath);
		}
		else if (define == eAssetDefine::_TEXTURE2DARRAY)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_TEXTURE2DARRAY)>(tFilePath);
		}
		else if (define == eAssetDefine::_TEXTURECUBE)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_TEXTURECUBE)>(tFilePath);
		}
		else if (define == eAssetDefine::_FONT)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_FONT)>(tFilePath);
		}
		else if (define == eAssetDefine::_CUBEMAP)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_CUBEMAP)>(tFilePath);
		}
		else if (define == eAssetDefine::_3DMODEL)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_3DMODEL)>(tFilePath);
		}
		else if (define == eAssetDefine::_RENDER_VERTEXSHADER)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_RENDER_VERTEXSHADER)>(tFilePath);
		}
		else if (define == eAssetDefine::_RENDER_PIXELSHADER)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_RENDER_PIXELSHADER)>(tFilePath);
		}
		else if (define == eAssetDefine::_RENDERMATERIAL)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_RENDERMATERIAL)>(tFilePath);
		}
		else if (define == eAssetDefine::_ANIMATION)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_ANIMATION)>(tFilePath);
		}
		else
		{
			assert(false);
			//여기까지 와도 안된다.
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_TEXTURE2D)>(tFilePath);
		}
	}

	bool GraphicsResourceManager::IsExistResource(const std::string& path)
	{
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(path);

		auto res = std::find_if(_resources.begin(), _resources.end(),
			[&tFilePath](const std::pair<std::string,std::weak_ptr<Pg::Data::Resources::GraphicsResource>>& val)
			-> bool {return (val.first == tFilePath); });

		bool tIsFound = (res != _resources.end()) ? true : false;
		return tIsFound;
	}

	bool GraphicsResourceManager::IsExistResourceByName(const std::string& name)
	{
		for (auto& it : _resources)
		{
			std::filesystem::path tPath = it.first;
			std::string tVal = tPath.filename().string();

			if (tVal.compare(name) == 0)
			{
				//동일 이름의 리소스가 있을 경우.
				return true;
			}
			//if (it.first.find(name) != std::string::npos)
			//{
			//	//동일 이름의 리소스가 있을 경우.
			//	return true;
			//}
		}

		//동일 이름의 리소스가 없을 경우.
		return false;
	}

	std::map<std::string, Pg::Data::Enums::eAssetDefine>* GraphicsResourceManager::GetSecondaryResources()
	{
		if (_toAddSecondaryResourcesMap.empty())
		{
			return nullptr;
		}
		else
		{
			//뭐라도 있다는 얘기.
			return &_toAddSecondaryResourcesMap;
		}
	}

	void GraphicsResourceManager::ClearSecondaryResourcesList()
	{
		_toAddSecondaryResourcesMap.clear();
	}

	void GraphicsResourceManager::AddSecondaryResource(const std::string& path, Pg::Data::Enums::eAssetDefine define)
	{
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(path);

		//AssetManager한테 이러이러한 애셋이 외부적으로 추가되었다고 알리는 과정!
		_toAddSecondaryResourcesMap.insert(std::make_pair(tFilePath, define));
	}

	RenderTexture2D* GraphicsResourceManager::GetDefaultTexture(eAssetTextureType textureType, eSizeTexture textureSize)
	{
		using namespace Pg::Defines;

		//256/512/1024/2048
		const std::string* tDiffuseTexture[4] = { &ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_256, &ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_512, &ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_1024, &ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_2048 };
		const std::string* tNormalTexture[4] = { &ASSET_DEFAULT_NORMAL_TEXTURE_PATH_256, &ASSET_DEFAULT_NORMAL_TEXTURE_PATH_512, &ASSET_DEFAULT_NORMAL_TEXTURE_PATH_1024, &ASSET_DEFAULT_NORMAL_TEXTURE_PATH_2048 };
		const std::string* tArmTexture[4] = { &ASSET_DEFAULT_ARM_TEXTURE_PATH_256, &ASSET_DEFAULT_ARM_TEXTURE_PATH_512, &ASSET_DEFAULT_ARM_TEXTURE_PATH_1024, &ASSET_DEFAULT_ARM_TEXTURE_PATH_2048 };
		const std::string* tSpecularTexture[4] = { &ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_256, &ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_512, &ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_1024, &ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_2048 };

		unsigned int tIndex = (unsigned int)textureSize;

		switch (textureType)
		{
		case PG_TextureType_DIFFUSE:
		{
			auto tRes = GetResource(*(tDiffuseTexture[tIndex]), Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
			return static_cast<RenderTexture2D*>(tRes.get());
		}
		break;
		case PG_TextureType_NORMALS:
		{
			auto tRes = GetResource(*(tNormalTexture[tIndex]), Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
			return static_cast<RenderTexture2D*>(tRes.get());
		}
		break;
		case PG_TextureType_ARM: //Metallic인데 Stolen Youth 내부 컨벤션.
		{
			auto tRes = GetResource(*(tArmTexture[tIndex]), Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
			return static_cast<RenderTexture2D*>(tRes.get());
		}
		break;
		case PG_TextureType_SPECULAR:
		{
			auto tRes = GetResource(*(tSpecularTexture[tIndex]), Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
			return static_cast<RenderTexture2D*>(tRes.get());
		}
		break;
		default:
		{
			assert(false && "아직 해당 종류 대해서는 디폴트 텍스쳐 준비되지 않았음!");
		}
		break;
		}
	}

	std::shared_ptr<Pg::Data::Resources::GraphicsResource> GraphicsResourceManager::GetResourceByName(const std::string& name, Pg::Data::Enums::eAssetDefine define)
	{
		std::string tFullPath = "";
		for (auto& it : _scopeResourceMap)
		{
			//if ((it.first.find(name) != std::string::npos) && (it.second->GetAssetDefine() == define))
			//{
			//	//동일 이름의 리소스 w/ 동일한 File Path가 있을 경우.
			//	tFullPath = it.first;
			//	break;
			//}

			std::filesystem::path tPath = it.first;
			std::string tVal = tPath.filename().string();

			if (tVal.compare(name) == 0 && (it.second->GetAssetDefine() == define))
			{
				//동일 이름의 리소스가 있을 경우.
				tFullPath = it.first;
			}
		}
	
		if (tFullPath.empty())
		{
			assert(false && "없는 리소스를 가지고 오려고 함!");
		}


		return GetResource(tFullPath, define);
	}

	std::vector<std::shared_ptr<Pg::Data::Resources::GraphicsResource>> GraphicsResourceManager::GetAllResourcesByDefine(Pg::Data::Enums::eAssetDefine define)
	{
		std::vector<std::shared_ptr<Pg::Data::Resources::GraphicsResource>> tRet;

		for (auto& it : _resources)
		{
			auto res = it.second.lock();
			assert(res);
			if (res->GetAssetDefine() == define)
			{
				tRet.push_back(res);
			}
		}

		return tRet;
	}

	std::string GraphicsResourceManager::GetResourcePathByName(const std::string& name, Pg::Data::Enums::eAssetDefine define)
	{
		std::string tFullPath = "";
		for (auto& it : _scopeResourceMap)
		{
			std::filesystem::path tPath = it.first;
			std::string tVal = tPath.filename().string();

			if ((tVal.compare(name) == 0) && (it.second->GetAssetDefine() == define))
			{
				tFullPath = it.first;
				break;
			}
		}

		if (tFullPath.empty())
		{
			assert(false && "없는 리소스 파일 이름의 전체 파일 경로를 가지고 오려고 함!");
		}

		return tFullPath;
	}

	bool GraphicsResourceManager::IsExistDefaultMaterialByMeshName(const std::string& name)
	{
		return IsExistResourceByName(Pg::Graphics::Helper::GraphicsResourceHelper::GetDefaultMaterialNameFromMeshName(name));
	}

	bool GraphicsResourceManager::IsExistResourceByNameType(const std::string& name, Pg::Data::Enums::eAssetDefine define)
	{
		for (auto& it : _scopeResourceMap)
		{
			std::filesystem::path tPath = it.first;
			std::string tVal = tPath.filename().string();

			if ((tVal.compare(name) == 0) && (it.second->GetAssetDefine() == define))
			{
				//동일 이름의 리소스 w/ 동일한 File Path가 있을 경우.
				return true;
			}
		}
		return false;
	}

	

}
