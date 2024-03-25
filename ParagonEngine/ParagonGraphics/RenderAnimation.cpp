#include "RenderAnimation.h"
#include "AssetCombinedLoader.h"
#include "GraphicsResourceManager.h"

namespace Pg::Graphics
{
	RenderAnimation::RenderAnimation(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		//생성자가 호출된 시점에서 이미 새로운 모델 데이터가 중복없이 만들어진다고 확신됨.
		_animAssetData = std::make_unique<Animation_AssetData>();
	}

	RenderAnimation::~RenderAnimation()
	{

	}

	void RenderAnimation::InternalLoad()
	{
		//중복되었는지 찾는 것은 외부에서 이루어질 일!
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetCombinedLoader;

		//로드 및 구분.
		AssetCombinedLoader* tLoader = GraphicsResourceManager::Instance()->GetCombinedLoader();
		tLoader->LoadAnimation(_filePath, this);
	}

	void RenderAnimation::InternalUnload()
	{

	}

}
