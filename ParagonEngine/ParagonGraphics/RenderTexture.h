#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"
#include "TextureExtension.h"

#include <string>
/// <summary>
/// 모든 RenderTexture의 부모 클래스. 이 형태로 직접적인 객체가 만들어지면 안됨.
/// </summary>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace Pg::Graphics
{
	namespace Loader
	{
		class AssetBasic2DLoader;
	}
}
namespace Pg::Graphics
{
	class RenderTexture : public Pg::Data::Resources::GraphicsResource
	{
		friend class Pg::Graphics::Loader::AssetBasic2DLoader;
	public:
		RenderTexture(Pg::Data::Enums::eAssetDefine define, const std::string& typeInfoStr, const std::string& filePath);
		~RenderTexture();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		ID3D11ShaderResourceView*& GetSRV();
		ID3D11Resource*& GetResource();

		std::wstring GetFilePathWide();

		//Texture Extension 반환.
		eTextureExtension GetTextureExtension() { return _textureExt; }
		
	private:
		//Load하는 로직이 이를 채워야 한다!
		ID3D11ShaderResourceView* _textureSRV = nullptr;
		ID3D11Resource* _textureBuffer = nullptr;

		eTextureExtension _textureExt = _UNMANAGED;
		//만약 PNG 등 WIC라면, 해당 정보가 실제로 쓰일 것.
		bool _isIgnoreSRGB{ false };
	};
}


