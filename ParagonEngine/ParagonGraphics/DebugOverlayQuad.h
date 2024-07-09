#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <string>

namespace Pg::Graphics
{
	class SystemVertexShader;
	class SystemPixelShader;
	class GBufferRender;
	class LowDX11Storage;

	class DebugOverlayQuad
	{
	public:
		DebugOverlayQuad(const std::wstring& psPath, D3D11_VIEWPORT vp, DirectX::XMFLOAT2 size);
		
		void Render();
	private:
		void BindVertexIndexBuffer();
		
	private:
		std::unique_ptr<SystemVertexShader> _vertexShader;
		std::unique_ptr<SystemPixelShader> _pixelShader;

		DirectX::XMFLOAT2 _size;
		std::unique_ptr<GBufferRender> _smallGBufQuad;
		LowDX11Storage* _DXStorage;
		D3D11_VIEWPORT _vp;
	};
}


