#include "RenderTexture2D.h"
#include "AssetBasic2DLoader.h"
#include "GraphicsResourceManager.h"
#include <typeinfo>

namespace Pg::Graphics
{
	RenderTexture2D::RenderTexture2D(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) : 
		GraphicsResource(define, typeid(this).name(), filePath)
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

	ID3D11ShaderResourceView*& RenderTexture2D::GetSRV()
	{
		return _textureSRV;
	}

	ID3D11Resource*& RenderTexture2D::GetResource()
	{
		return _textureBuffer;
	}

	std::wstring RenderTexture2D::GetFilePath()
	{
		std::wstring _wstring;
		_wstring.assign(_filePath.begin(), _filePath.end());

		return _wstring;
	}

}