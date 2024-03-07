#include "RenderAnimation.h"
#include "AssetBasic3DLoader.h"
#include "GraphicsResourceManager.h"
#include "AssimpBufferParser.h"

namespace Pg::Graphics
{
	RenderAnimation::RenderAnimation(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		//생성자가 호출된 시점에서 이미 새로운 모델 데이터가 중복없이 만들어진다고 확신됨.
	}

	RenderAnimation::~RenderAnimation()
	{

	}

	void RenderAnimation::InternalLoad()
	{
		//중복되었는지 찾는 것은 외부에서 이루어질 일!

		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetBasic3DLoader;

		//로드 및 구분.
		AssetBasic3DLoader* t3DLoader = GraphicsResourceManager::Instance()->GetBasic3DLoader();
		t3DLoader->LoadAnimation(_filePath, this);
	}

	void RenderAnimation::InternalUnload()
	{

	}

}
