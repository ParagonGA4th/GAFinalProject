#include "RenderFont.h"
#include "AssetBasic2DLoader.h"
#include "GraphicsResourceManager.h"


namespace Pg::Graphics
{
	RenderFont::RenderFont(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		//�����ڰ� ȣ��� �������� �̹� ���ο� �� �����Ͱ� �ߺ����� ��������ٰ� Ȯ�ŵ�.
	}

	RenderFont::~RenderFont()
	{

	}

	void RenderFont::InternalLoad()
	{
		//�ߺ��Ǿ����� ã�� ���� �ܺο��� �̷���� ��!
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetBasic2DLoader;

		AssetBasic2DLoader* t2DLoader = GraphicsResourceManager::Instance()->GetBasic2DLoader();
		t2DLoader->LoadFont(_filePath, this);
	}

	void RenderFont::InternalUnload()
	{

	}

}