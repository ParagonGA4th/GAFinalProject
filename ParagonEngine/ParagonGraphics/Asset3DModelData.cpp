#include "Asset3DModelData.h"
#include "AssetBasic3DLoader.h"
#include "GraphicsResourceManager.h"
namespace Pg::Graphics
{

	Asset3DModelData::Asset3DModelData(Pg::Core::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		//생성자가 호출된 시점에서 이미 새로운 모델 데이터가 중복없이 만들어진다고 확신됨.
	}

	Asset3DModelData::~Asset3DModelData()
	{

	}

	void Asset3DModelData::InternalLoad()
	{
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetBasic3DLoader;

		AssetBasic3DLoader* t3DLoader = GraphicsResourceManager::Instance()->GetBasic3DLoader();
		this->_isSkinned = t3DLoader->IsModelSkinned(_filePath);
		t3DLoader->Load3DModel(_isSkinned, _filePath);

	}

	void Asset3DModelData::InternalUnload()
	{

	}

}