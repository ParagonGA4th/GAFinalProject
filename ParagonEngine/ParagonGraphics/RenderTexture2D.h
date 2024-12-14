#pragma once
#include "RenderTexture.h"

/// <summary>
/// ���� Texture2D�� 1��1 �����Ǵ� Ŭ����. ���ӿ������� ����� Texture2D�� �����Ǿ� �����ȴ�.
/// == Image
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
	class RenderTexture2D : public RenderTexture
	{
		friend class Pg::Graphics::Loader::AssetBasic2DLoader;
	public:
		RenderTexture2D(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderTexture2D();

		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		unsigned int GetFileWidth();
		unsigned int GetFileHeight();

	private:
		unsigned int _fileWidth = 0;
		unsigned int _fileHeight = 0;
	};

}

