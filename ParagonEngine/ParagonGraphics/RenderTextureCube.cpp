#include "RenderTextureCube.h"
#include "AssetBasic2DLoader.h"
#include "GraphicsResourceManager.h"
#include <typeinfo>

namespace Pg::Graphics
{
	RenderTextureCube::RenderTextureCube(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		RenderTexture(define, typeid(this).name(), filePath)
	{
		//�����ڰ� ȣ��� �������� �̹� ���ο� �� �����Ͱ� �ߺ����� ��������ٰ� Ȯ�ŵ�.
	}

	RenderTextureCube::~RenderTextureCube()
	{

	}

	void RenderTextureCube::InternalLoad()
	{
		//�ߺ��Ǿ����� ã�� ���� �ܺο��� �̷���� ��!
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetBasic2DLoader;

		AssetBasic2DLoader* t2DLoader = GraphicsResourceManager::Instance()->GetBasic2DLoader();
		t2DLoader->LoadTextureCube(_filePath, this);
	}

	void RenderTextureCube::InternalUnload()
	{

	}
}