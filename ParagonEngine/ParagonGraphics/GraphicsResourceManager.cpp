#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"
#include "AssetBasic2DLoader.h"
#include "AssetBasic3DLoader.h"

#include "RenderTexture2D.h" //디폴트 텍스쳐 로드 때문에.
#include "AssetTextureType.h"

#include "../ParagonData/ParagonDefines.h" 
#include "../ParagonUtil/ResourceHelper.h"

#include <cassert>
#include <algorithm>

namespace Pg::Graphics::Manager
{
	GraphicsResourceManager::GraphicsResourceManager()
	{
		Pg::Graphics::Helper::GraphicsResourceHelper::Initialize();

		_asset2DLoader = std::make_unique<Pg::Graphics::Loader::AssetBasic2DLoader>();
		_asset3DLoader = std::make_unique<Pg::Graphics::Loader::AssetBasic3DLoader>();
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

	void GraphicsResourceManager::LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define)
	{
		//LoadResource 호출되었다는 것 = Asset이 아직 없다는 말.
		//Can Compile!
		//ASSETDEFINE2TYPE_MEMFUNC_2PARAMS(define, this, GraphicsResourceManager::CreateResource, filePath, define);
		
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filePath);

		using Pg::Data::Enums::eAssetDefine;
		if (define == eAssetDefine::_2DTEXTURE)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_2DTEXTURE)>(tFilePath, define);
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
		else
		{
			assert(false); //여기까지 와도 안된다.
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_2DTEXTURE)>(tFilePath, define);
		}
	}

	void GraphicsResourceManager::UnloadResource(const std::string& filePath)
	{
		//Load와 달리, 동시에 두 개의 리소스 매니저가 동시에 호출된다. //지우지 못했어도 오류 반환하지 말자.
	}

	std::shared_ptr<Pg::Data::Resources::GraphicsResource> GraphicsResourceManager::GetResource(const std::string& path, Pg::Data::Enums::eAssetDefine define)
	{
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(path);

		using Pg::Data::Enums::eAssetDefine;

		if (define == eAssetDefine::_2DTEXTURE)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_2DTEXTURE)>(tFilePath);
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
		else
		{
			assert(false);
			//여기까지 와도 안된다.
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_2DTEXTURE)>(tFilePath);
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

	RenderTexture2D* GraphicsResourceManager::GetDefaultTexture(eAssetTextureType textureType)
	{
		switch (textureType)
		{
		case PG_TextureType_DIFFUSE:
		{
			auto tRes = GetResource(Pg::Defines::ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH, Pg::Data::Enums::eAssetDefine::_2DTEXTURE);
			return static_cast<RenderTexture2D*>(tRes.get());
		}
		break;
		case PG_TextureType_NORMALS:
		{
			auto tRes = GetResource(Pg::Defines::ASSET_DEFAULT_NORMAL_TEXTURE_PATH, Pg::Data::Enums::eAssetDefine::_2DTEXTURE);
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
		for (auto& it : _resources)
		{
			if (it.first.find(name) != std::string::npos)
			{
				tFullPath = it.first;
				break;
			}
		}
	
		if (tFullPath.empty())
		{
			assert(false && "없는 리소스를 가지고 오려고 함!");
		}

		return GetResource(tFullPath, define);
	}
}
