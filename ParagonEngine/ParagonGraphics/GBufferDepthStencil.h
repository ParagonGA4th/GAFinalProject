#pragma once
#include "../ParagonData/GameConstantData.h"

class ID3D11Texture2D;
class ID3D11DepthStencilView;
class ID3D11ShaderResourceView;
class ID3D11DepthStencilState;

enum DXGI_FORMAT;
struct D3D11_DEPTH_STENCIL_DESC;

/// <summary>
/// G-Buffer 중, 일반 DepthStencil에 대응하는 클래스.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class GBufferDepthStencil
	{
	public:
		GBufferDepthStencil(D3D11_DEPTH_STENCIL_DESC* dsDesc = nullptr, DirectX::XMFLOAT2 widthHeight = Pg::Data::GameConstantData::WIDTH_HEIGHT_PAIR);
		~GBufferDepthStencil();
	public:
		ID3D11DepthStencilView*& GetDSV();
		ID3D11ShaderResourceView*& GetSRV();
		ID3D11DepthStencilState*& GetDSState();

	private:
		void CreateBuffer(DXGI_FORMAT format);
		void CreateDSV(DXGI_FORMAT format);
		void CreateSRV(DXGI_FORMAT format);
		void CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dsDesc);

	private:
		ID3D11Texture2D* _Buffer;
		ID3D11DepthStencilView* _DSV;
		ID3D11ShaderResourceView* _SRV;
		ID3D11DepthStencilState* _dsState;
		//용도가 달라짐에 따라서, DepthStencilState역시 필요하다.

	private:
		LowDX11Storage* _DXStorage;
		DirectX::XMFLOAT2 _widthHeight;
	};
}
