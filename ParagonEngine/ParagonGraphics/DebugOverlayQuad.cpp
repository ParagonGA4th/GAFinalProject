#include "DebugOverlayQuad.h"
#include "GBufferRender.h"
#include "GeometryGenerator.h"
#include "LowDX11Storage.h"
#include "SystemVertexShader.h"
#include "LayoutDefine.h"
#include "SystemPixelShader.h"
#include "../ParagonHelper/ResourceHelper.h"
#include "../ParagonData/ParagonDefines.h"

namespace Pg::Graphics
{
	using Pg::Util::Helper::ResourceHelper;

	DebugOverlayQuad::DebugOverlayQuad(const std::wstring& psPath, D3D11_VIEWPORT vp, DirectX::XMFLOAT2 size) :
		_size(size), _DXStorage(LowDX11Storage::GetInstance()), _vp(vp)
	{
		//Small GBuf Quad.
		_smallGBufQuad = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT, size);

		//Vertex - Pixel Shader 만들기.
		using Pg::Util::Helper::ResourceHelper;
		using namespace Pg::Defines;

		//개별적으로 쓰일 Vertex Shader 별도로 분리.
		_vertexShader = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(POSTPROCESSING_DEFAULT_VS_DIRECTORY), LayoutDefine::GetDeferredQuadLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_pixelShader = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(psPath));
	}

	void DebugOverlayQuad::Render()
	{
		//RenderTarget은 계속 Backbuffer에서 유지된다.
		BindVertexIndexBuffer();

		_DXStorage->_deviceContext->RSSetViewports(1, &_vp);

		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), nullptr);

		//이미 있는 셰이더 출력할 것.
		_vertexShader->Bind();
		_pixelShader->Bind();

		//DrawIndexed.
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);

		//Unbind.
		_vertexShader->Unbind();
		_pixelShader->Unbind();

		_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_defaultViewport));
	}

	void DebugOverlayQuad::BindVertexIndexBuffer()
	{
		//BindVerteexIndexBuffer
		assert(GeometryGenerator::_QUAD_VB != nullptr);
		assert(GeometryGenerator::_QUAD_IB != nullptr);

		// Bind Buffers
		UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(GeometryGenerator::_QUAD_VB), &stride, &offset);
		_DXStorage->_deviceContext->IASetIndexBuffer(GeometryGenerator::_QUAD_IB, DXGI_FORMAT_R32_UINT, 0);
	}

	

}

