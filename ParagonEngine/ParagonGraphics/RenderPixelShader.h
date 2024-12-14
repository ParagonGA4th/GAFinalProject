#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"

#include "DX11Headers.h"

#include <memory>
#include <string>
#include <vector>

/// <summary>
/// Ŭ���̾�Ʈ���� ����Ǵ�,
/// ������ ���������ο� �Ŀ� ���ε��Ǵ� ���̴��� �����ϱ� ���� ����.
/// ���ӿ��� Ŭ���̾�Ʈ���� ������ Pixel Shader�� ��Ī�Ǵ� ���.
/// </summary>

namespace Pg::Graphics
{
	namespace Loader
	{
		class AssetCombinedLoader;
	}
}

namespace Pg::Graphics
{
	class LowDX11Storage;

	class RenderPixelShader : public Pg::Data::Resources::GraphicsResource
	{
		friend class Pg::Graphics::Loader::AssetCombinedLoader;
	public:
		RenderPixelShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		virtual ~RenderPixelShader();

	public:
		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

	public:
		void Bind();
		void Unbind();
		ID3D11PixelShader* GetPixelShader();

	private:
		LowDX11Storage* _DXStorage = nullptr;

		// ���̴� ����Ʈ�ڵ带 ����� ����
		ID3DBlob* _byteCode = nullptr;
		ID3D11PixelShader* _shader = nullptr;
	};
}
