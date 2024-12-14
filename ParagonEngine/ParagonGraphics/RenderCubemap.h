#pragma once
#include "DX11Headers.h"

#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"
#include <memory>
#include <string>

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

namespace Pg::Data
{
	struct CameraData;
}

namespace Pg::Graphics
{
	class LowDX11Storage;

	namespace Loader
	{
		class AssetBasic2DLoader;
	}
}

namespace Pg::Graphics
{
	class RenderCubemap : public Pg::Data::Resources::GraphicsResource
	{
		friend class Pg::Graphics::Loader::AssetBasic2DLoader;
	public:
		RenderCubemap(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderCubemap();

	public:
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		//�ϴ��� ���δ� 0,0,0�� �׸��� �����̴�, �ϰ������� ó���ص� ����.
		void UpdateConstantBuffers(Pg::Data::CameraData* camData);
		void BindConstantBuffers();
		void BindAdditionalResources();
		void Render();
		void UnbindAdditionalResources();
		void UnbindConstantBuffers();

	private:
		void CreateConstantBuffers();

	public:
		//CubeMapCB _cbData;

		ID3D11Buffer* GetVB() { return _VB; }
		ID3D11Buffer* GetIB() { return _IB; }
		ID3D11ShaderResourceView*& GetSRV() { return _srv; }

		//�ϴ��� Cubemap ������Ʈ�� ���� ������ CBuffer�� ����.
		struct CB
		{
			DirectX::XMFLOAT4X4 worldMatrix;
			DirectX::XMFLOAT4X4 viewProjMatrix;
		};

	private:
		ID3D11Buffer* _VB = nullptr;
		ID3D11Buffer* _IB = nullptr;
		ID3D11ShaderResourceView* _srv = nullptr;
		static unsigned int _indexCount;

		//Constant Buffer.
		D3D11_SUBRESOURCE_DATA _cbSubresData;
		ID3D11Buffer* _cBuffer;
		ID3D11Buffer* _nullBuffer = nullptr;
		RenderCubemap::CB _cbData;

	private:
		LowDX11Storage* _DXStorage = nullptr;
	};
}
