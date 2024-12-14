#include "RenderTexture2D.h"
#include "AssetBasic2DLoader.h"
#include "GraphicsResourceManager.h"
#include "RenderTexture.h"
#include <typeinfo>

namespace Pg::Graphics
{
	RenderTexture2D::RenderTexture2D(Pg::Data::Enums::eAssetDefine define, const std::string& filePath)
		: RenderTexture(define, typeid(this).name(), filePath)
	{
		//�����ڰ� ȣ��� �������� �̹� ���ο� �� �����Ͱ� �ߺ����� ��������ٰ� Ȯ�ŵ�.
	}

	RenderTexture2D::~RenderTexture2D()
	{

	}

	void RenderTexture2D::InternalLoad()
	{
		//�ߺ��Ǿ����� ã�� ���� �ܺο��� �̷���� ��!
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetBasic2DLoader;

		AssetBasic2DLoader* t2DLoader = GraphicsResourceManager::Instance()->GetBasic2DLoader();
		t2DLoader->LoadTexture2D(_filePath, this);
	}

	void RenderTexture2D::InternalUnload()
	{

	}

	unsigned int RenderTexture2D::GetFileWidth()
	{
		return _fileWidth;
	}

	unsigned int RenderTexture2D::GetFileHeight()
	{
		return _fileHeight;
	}

}