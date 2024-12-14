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
		//�����ڰ� ȣ��� �������� �̹� ���ο� �� �����Ͱ� �ߺ����� ��������ٰ� Ȯ�ŵ�.
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
		//�ߺ��Ǿ����� ã�� ���� �ܺο��� �̷���� ��!

		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetBasic3DLoader;

		//�ε� �� ����.
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