#include "RenderTexture1D.h"
#include "AssetBasic2DLoader.h"
#include "GraphicsResourceManager.h"
#include <typeinfo>

namespace Pg::Graphics
{
	RenderTexture1D::RenderTexture1D(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		RenderTexture(define, typeid(this).name(), filePath)
	{
		//�����ڰ� ȣ��� �������� �̹� ���ο� �� �����Ͱ� �ߺ����� ��������ٰ� Ȯ�ŵ�.
	}

	RenderTexture1D::~RenderTexture1D()
	{

	}

	void RenderTexture1D::InternalLoad()
	{
		//�ߺ��Ǿ����� ã�� ���� �ܺο��� �̷���� ��!
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetBasic2DLoader;

		AssetBasic2DLoader* t2DLoader = GraphicsResourceManager::Instance()->GetBasic2DLoader();
		t2DLoader->LoadTexture1D(_filePath, this);
	}

	void RenderTexture1D::InternalUnload()
	{

	}
}
