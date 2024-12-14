#include "RenderAnimation.h"
#include "AssetCombinedLoader.h"
#include "GraphicsResourceManager.h"

namespace Pg::Graphics
{
	RenderAnimation::RenderAnimation(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		//�����ڰ� ȣ��� �������� �̹� ���ο� �� �����Ͱ� �ߺ����� ��������ٰ� Ȯ�ŵ�.
		_animAssetData = std::make_unique<Animation_AssetData>();
	}

	RenderAnimation::~RenderAnimation()
	{

	}

	void RenderAnimation::InternalLoad()
	{
		//�ߺ��Ǿ����� ã�� ���� �ܺο��� �̷���� ��!
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetCombinedLoader;

		//�ε� �� ����.
		AssetCombinedLoader* tLoader = GraphicsResourceManager::Instance()->GetCombinedLoader();
		tLoader->LoadAnimation(_filePath, this);
	}

	void RenderAnimation::InternalUnload()
	{

	}

}
