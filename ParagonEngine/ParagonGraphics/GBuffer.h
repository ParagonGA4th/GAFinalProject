#pragma once

class ID3D11Texture2D;
class ID3D11RenderTargetView;
class ID3D11ShaderResourceView;

namespace Pg::Graphics
{
	class LowDX11Storage;

	class GBuffer
	{
	public:
		GBuffer();

	public:
		void Render();

	private:
		void Bind();
		void Unbind();

	private:
		void CreateBuffer();
		void CreateRTV();
		void CreateSRV();

	private:
		ID3D11Texture2D* _Buffer;
		ID3D11RenderTargetView* _RTV;
		ID3D11ShaderResourceView* _SRV;

	private:
		LowDX11Storage* _DXStorage;
	};
}
