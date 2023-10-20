#pragma once

class ID3D11Texture2D;
class ID3D11RenderTargetView;
class ID3D11ShaderResourceView;

enum DXGI_FORMAT;

namespace Pg::Graphics
{
	class LowDX11Storage;

	class GBuffer
	{
	public:
		GBuffer(DXGI_FORMAT BufferFormat, DXGI_FORMAT ViewFormat);

	public:
		void Render();

	public:
		void Bind();
		void Unbind();

	private:
		void CreateBuffer(DXGI_FORMAT format);
		void CreateRTV(DXGI_FORMAT format);
		void CreateSRV(DXGI_FORMAT format);

	public:
		ID3D11RenderTargetView* GetRTV();
		ID3D11ShaderResourceView* GetSRV();

	private:
		ID3D11Texture2D* _Buffer;
		ID3D11RenderTargetView* _RTV;
		ID3D11ShaderResourceView* _SRV;

	private:
		LowDX11Storage* _DXStorage;
	};
}
