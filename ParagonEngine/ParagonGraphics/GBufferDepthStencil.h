#pragma once

class ID3D11Texture2D;
class ID3D11DepthStencilView;
class ID3D11ShaderResourceView;
class ID3D11DepthStencilState;

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
		ID3D11DepthStencilState* GetDSState();
	private:
		void CreateBuffer(DXGI_FORMAT format);
		void CreateDSV(DXGI_FORMAT format);
		void CreateSRV(DXGI_FORMAT format);
		void CreateDepthStencilState();

	private:
		ID3D11Texture2D* _Buffer;
		ID3D11DepthStencilView* _DSV;
		ID3D11ShaderResourceView* _SRV;
		ID3D11DepthStencilState* _dsState;
		//용도가 달라짐에 따라서, DepthStencilState역시 필요하다.

	private:
		LowDX11Storage* _DXStorage;
	};
}
