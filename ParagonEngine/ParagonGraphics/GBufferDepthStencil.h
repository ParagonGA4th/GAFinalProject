#pragma once

class ID3D11Texture2D;
class ID3D11DepthStencilView;
class ID3D11ShaderResourceView;

enum DXGI_FORMAT;

/// <summary>
/// G-Buffer 중, 일반 DepthStencil에 대응하는 클래스.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class GBufferDepthStencil
	{
	public:
		GBufferDepthStencil(DXGI_FORMAT BufferFormat, DXGI_FORMAT ViewFormat);
		~GBufferDepthStencil();
	public:
		ID3D11DepthStencilView* GetDSV();
		ID3D11ShaderResourceView* GetSRV();

	private:
		void CreateBuffer(DXGI_FORMAT format);
		void CreateDSV(DXGI_FORMAT format);
		void CreateSRV(DXGI_FORMAT format);

	private:
		ID3D11Texture2D* _Buffer;
		ID3D11DepthStencilView* _DSV;
		ID3D11ShaderResourceView* _SRV;

	private:
		LowDX11Storage* _DXStorage;
	};
}
