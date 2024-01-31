#pragma once

class ID3D11Texture2D;
class ID3D11RenderTargetView;
class ID3D11ShaderResourceView;

enum DXGI_FORMAT;

/// <summary>
/// G-Buffer 중, 일반 RenderTarget에 대응하는 클래스.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class GBufferRender
	{
	public:
		GBufferRender(DXGI_FORMAT BufferFormat, DXGI_FORMAT ViewFormat);
		~GBufferRender();
	public:
		ID3D11RenderTargetView*& GetRTV();
		ID3D11ShaderResourceView*& GetSRV();

	private:
		void CreateBuffer(DXGI_FORMAT format);
		void CreateRTV(DXGI_FORMAT format);
		void CreateSRV(DXGI_FORMAT format);

	private:
		ID3D11Texture2D* _Buffer;
		ID3D11RenderTargetView* _RTV;
		ID3D11ShaderResourceView* _SRV;

	private:
		LowDX11Storage* _DXStorage;
	};
}
