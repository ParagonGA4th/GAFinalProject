#include "Asset3DModelData.h"
#include "AssetModelDataDefine.h"
#include "AssetBasic3DLoader.h"
#include "GraphicsResourceManager.h"
#include "MaterialCluster.h"
#include "AssimpBufferParser.h"
#include "AnimationParser.h"
#include <cassert>
#include <d3d11.h>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::AssimpBufferParser;

	Asset3DModelData::Asset3DModelData(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		//생성자가 호출된 시점에서 이미 새로운 모델 데이터가 중복없이 만들어진다고 확신됨.
	}

	Asset3DModelData::~Asset3DModelData()
	{
		if (_assetSceneData != nullptr)
		{
			delete _assetSceneData;
		}
		if (_assetSkinnedData != nullptr)
		{
			delete _assetSkinnedData;
		}
	}

	void Asset3DModelData::InternalLoad()
	{
		//중복되었는지 찾는 것은 외부에서 이루어질 일!

		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetBasic3DLoader;

		//로드 및 구분.
		AssetBasic3DLoader* t3DLoader = GraphicsResourceManager::Instance()->GetBasic3DLoader();
		t3DLoader->Load3DModelBuffer(_filePath, this);
	}

	void Asset3DModelData::InternalUnload()
	{

	}

	bool Asset3DModelData::IsSkinned()
	{
		return _isSkinned;
	}

	Pg::Graphics::MaterialCluster* Asset3DModelData::GetMaterialByIndex(short index)
	{
		assert(index >= 0 && index < Pg::Defines::MAX_MATERIAL_PER_MODEL);
		assert(_materialClusterList.size() > index);

		return _materialClusterList.at(index);
	}

	unsigned int Asset3DModelData::GetMaterialCount() const
	{
		return _materialClusterList.size();
	}

}