#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"
#include "AssetBasic2DLoader.h"
#include "AssetBasic3DLoader.h"

#include "../ParagonUtil/ResourceHelper.h"

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
		
		using Pg::Data::Enums::eAssetDefine;
		if (define == eAssetDefine::_2DTEXTURE)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_2DTEXTURE)>(filePath, define);
		}
		else if (define == eAssetDefine::_3DMODEL)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_3DMODEL)>(filePath, define);
		}
		else if (define == eAssetDefine::_RENDERMATERIAL)
		{
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_RENDERMATERIAL)>(filePath, define);
		}
		else
		{
			assert(false); //여기까지 와도 안된다.
			CreateResource<ASSETDEFINE_TYPE(eAssetDefine::_2DTEXTURE)>(filePath, define);
		}
	}

	void GraphicsResourceManager::UnloadResource(const std::string& filePath)
	{
		//Load와 달리, 동시에 두 개의 리소스 매니저가 동시에 호출된다. //지우지 못했어도 오류 반환하지 말자.
	}

	std::shared_ptr<Pg::Data::Resources::GraphicsResource> GraphicsResourceManager::GetResource(const std::string& path, Pg::Data::Enums::eAssetDefine define)
	{
		using Pg::Data::Enums::eAssetDefine;

		if (define == eAssetDefine::_2DTEXTURE)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_2DTEXTURE)>(path);
		}
		else if (define == eAssetDefine::_3DMODEL)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_3DMODEL)>(path);
		}
		else if (define == eAssetDefine::_RENDERMATERIAL)
		{
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_RENDERMATERIAL)>(path);
		}
		else
		{
			assert(false);
			//여기까지 와도 안된다.
			return GetResourceTemplated<ASSETDEFINE_TYPE(eAssetDefine::_2DTEXTURE)>(path);
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
	

}
