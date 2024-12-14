#include "DefaultQuadRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GeometryGenerator.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "RenderMaterial.h"
#include "DX11Headers.h"

#include "../ParagonHelper/ResourceHelper.h"

namespace Pg::Graphics
{
	DefaultQuadRenderPass::DefaultQuadRenderPass()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	DefaultQuadRenderPass::~DefaultQuadRenderPass()
	{

	}

	void DefaultQuadRenderPass::Initialize()
	{
		CreateShaders();
		CreateMaterialIndexConstantBuffer();
	}

	void DefaultQuadRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		//�ڽ��� ������ RenderTarget�� �޴´�.
		//_passRenderTargetView = carrier._quadMainRT->GetRTV();
		//
		////�ڽ��� ������ DepthStencil�� �޴´�.
		//_passDepthStencilView = carrier._dsv;
	}

	void DefaultQuadRenderPass::BindPass()
	{
		BindVertexIndexBuffer();

		//VertexShader / Pixel ����.
		_defaultVS->Bind();
		_defaultPS->Bind();

		BindMaterialIndexConstantBuffer();

		//// Bind RenderTarget
		//_DXStorage->_deviceContext->OMSetRenderTargets(1, &_passRenderTargetView, _passDepthStencilView);
	}

	void DefaultQuadRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
	}

	void DefaultQuadRenderPass::UnbindPass()
	{
		//VertexShader / Pixel ����.
		_defaultVS->Unbind();
		_defaultPS->Unbind();
	}

	void DefaultQuadRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void DefaultQuadRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{
		//�ϴ°� ������, �ǵ帮�� ���ƾ� �Ѵ�.
	}

	void DefaultQuadRenderPass::BindVertexIndexBuffer()
	{
		assert(GeometryGenerator::_QUAD_VB != nullptr);
		assert(GeometryGenerator::_QUAD_IB != nullptr);

		// Bind Buffers
		UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(GeometryGenerator::_QUAD_VB), &stride, &offset);
		_DXStorage->_deviceContext->IASetIndexBuffer(GeometryGenerator::_QUAD_IB, DXGI_FORMAT_R32_UINT, 0);
	}

	void DefaultQuadRenderPass::BindMaterialIndexConstantBuffer()
	{
		//Mapped Subresource �޸� Ŭ��.
		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HR(_DXStorage->_deviceContext->Map(_cbMatID, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

		//�ڽ��� Data ���� ������ �޾ƿ���.
		UINT* data = reinterpret_cast<UINT*>(res.pData);

		//Material �ο�.
		*(data) = _defaultMatID;

		_DXStorage->_deviceContext->Unmap(_cbMatID, 0);

		//���� Binding. (Pixel Shader 3�� �������ͷ� ����)
		_DXStorage->_deviceContext->PSSetConstantBuffers(3, 1, &_cbMatID);
	}

	void DefaultQuadRenderPass::CreateMaterialIndexConstantBuffer()
	{
		//ID�� �����͸� �޾ƿ´�.
		UINT* _cbData = &_defaultMatID;

		//Constant Buffer ��ü�� ����� �ڵ�.
		int sizeCB = (((sizeof(unsigned int) - 1) / 16) + 1) * 16;	// declspec ���� 16����Ʈ ������ �� �ִ�?
		assert(sizeCB % 16 == 0);

		D3D11_BUFFER_DESC tDesc;
		tDesc.ByteWidth = sizeCB; // ������۴� 16����Ʈ ����
		tDesc.Usage = D3D11_USAGE_DYNAMIC;
		tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA tSubResource;
		tSubResource.pSysMem = _cbData;

		HR(_DXStorage->_device->CreateBuffer(&tDesc, &tSubResource, &(_cbMatID)));
	}

	void DefaultQuadRenderPass::CreateShaders()
	{
		using Pg::Util::Helper::ResourceHelper;

		std::wstring tDefaultVSW = std::wstring().assign(Pg::Defines::DEFAULT_APPENDS_RENDER_VS_PATH.begin(), Pg::Defines::DEFAULT_APPENDS_RENDER_VS_PATH.end());
		_defaultVS = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(tDefaultVSW), LayoutDefine::GetDeferredQuadLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		std::wstring tDefaultPSW = std::wstring().assign(Pg::Defines::DEFAULT_APPENDS_RENDER_PS_PATH.begin(), Pg::Defines::DEFAULT_APPENDS_RENDER_PS_PATH.end());
		_defaultPS = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(tDefaultPSW));
	}

}