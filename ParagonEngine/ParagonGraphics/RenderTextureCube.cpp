#include "RenderTextureCube.h"
#include "AssetBasic2DLoader.h"
#include "GraphicsResourceManager.h"
#include <typeinfo>

namespace Pg::Graphics
{
	RenderTextureCube::RenderTextureCube(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		RenderTexture(define, typeid(this).name(), filePath)
	{
		//생성자가 호출된 시점에서 이미 새로운 모델 데이터가 중복없이 만들어진다고 확신됨.
	}

	RenderTextureCube::~RenderTextureCube()
	{

	}

	void RenderTextureCube::InternalLoad()
	{
		//중복되었는지 찾는 것은 외부에서 이루어질 일!
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetBasic2DLoader;

		AssetBasic2DLoader* t2DLoader = GraphicsResourceManager::Instance()->GetBasic2DLoader();
		t2DLoader->LoadTextureCube(_filePath, this);
	}

	void RenderTextureCube::InternalUnload()
	{

	}
}