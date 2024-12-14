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
		//생성자가 호출된 시점에서 이미 새로운 모델 데이터가 중복없이 만들어진다고 확신됨.
	}

	RenderTexture2D::~RenderTexture2D()
	{

	}

	void RenderTexture2D::InternalLoad()
	{
		//중복되었는지 찾는 것은 외부에서 이루어질 일!
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