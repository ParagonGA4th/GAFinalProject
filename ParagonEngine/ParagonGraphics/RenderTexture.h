#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"
#include "TextureExtension.h"

#include <string>
/// <summary>
/// ��� RenderTexture�� �θ� Ŭ����. �� ���·� �������� ��ü�� ��������� �ȵ�.
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

		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		ID3D11ShaderResourceView*& GetSRV();
		ID3D11Resource*& GetResource();

		std::wstring GetFilePathWide();

		//Texture Extension ��ȯ.
		eTextureExtension GetTextureExtension() { return _textureExt; }
		
	private:
		//Load�ϴ� ������ �̸� ä���� �Ѵ�!
		ID3D11ShaderResourceView* _textureSRV = nullptr;
		ID3D11Resource* _textureBuffer = nullptr;

		eTextureExtension _textureExt = _UNMANAGED;
		//���� PNG �� WIC���, �ش� ������ ������ ���� ��.
		bool _isIgnoreSRGB{ false };
	};
}


