#include "Asset3DModelData.h"
#include "AssetBasic3DLoader.h"
#include "GraphicsResourceManager.h"
#include "BufferParser.h"
#include <d3d11.h>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::BufferParser;

	Asset3DModelData::Asset3DModelData(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		//생성자가 호출된 시점에서 이미 새로운 모델 데이터가 중복없이 만들어진다고 확신됨.
	}

	Asset3DModelData::~Asset3DModelData()
	{
		if (_assetSceneData)
		{
			delete _assetSceneData;
		}
	}

	void Asset3DModelData::InternalLoad()
	{
		//중복되었는지 찾는 것은 외부에서 이루어질 일!

		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetBasic3DLoader;

		//로드 및 구분.
		AssetBasic3DLoader* t3DLoader = GraphicsResourceManager::Instance()->GetBasic3DLoader();
		t3DLoader->Load3DModel(_filePath, this);
		
		//실제로 DX11 버퍼 로드. (Static, Skinned 모두)
		BufferParser::Asset3DModelToD3DBuffer(_d3dBufferInfo, _isSkinned, _assetSceneData);
	}

	void Asset3DModelData::InternalUnload()
	{

	}

}