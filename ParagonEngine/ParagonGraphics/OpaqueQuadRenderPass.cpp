#include "OpaqueQuadRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GeometryGenerator.h"
#include "RenderVertexShader.h"
#include "RenderPixelShader.h"
#include "RenderMaterial.h"
#include "DX11Headers.h"

namespace Pg::Graphics
{
	OpaqueQuadRenderPass::OpaqueQuadRenderPass(RenderMaterial* renderMat) :
		_renderMaterial(renderMat)
	{
		_DXStorage = LowDX11Storage::GetInstance();

		//생성자 순서대로 호출되는 매커닉 활용 위해서, 일단은 생성자에. 
		GenerateQuadBuffer();
	}

	OpaqueQuadRenderPass::~OpaqueQuadRenderPass()
	{

	}

	void OpaqueQuadRenderPass::Initialize()
	{
		CreateMaterialIndexConstantBuffer();
	}

	void OpaqueQuadRenderPass::ReceiveRequiredElements(ID3D11RenderTargetView** rtvArray, unsigned int rtvCount, 
		ID3D11ShaderResourceView** srvArray, unsigned int srvCount, ID3D11DepthStencilView* dsv)
	{
		assert(rtvCount == 1);

		//자신이 렌더할 RenderTarget을 받는다.
		_passRenderTargetView = rtvArray[0];

		//자신이 렌더할 DepthStencil을 받는다.
		_passDepthStencilView = dsv;
	}

	void OpaqueQuadRenderPass::BindPass()
	{
		BindVertexIndexBuffer();
		_renderMaterial->Bind();
		BindMaterialIndexConstantBuffer();

		// Unbind RenderTarget
		_DXStorage->_deviceContext->OMSetRenderTargets(1, nullptr, _passDepthStencilView);
	}

	void OpaqueQuadRenderPass::RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
	}

	void OpaqueQuadRenderPass::UnbindPass()
	{
		_renderMaterial->Unbind();

		// Unbind RenderTarget
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &_passRenderTargetView, _passDepthStencilView);
	}

	void OpaqueQuadRenderPass::GenerateQuadBuffer()
	{
		GeometryGenerator::MeshData_PosNormalTex tMeshData;
		GeometryGenerator::GenerateFullscreenQuad(tMeshData);

		// Buffer Description
		D3D11_BUFFER_DESC VBDesc;
		VBDesc.Usage = D3D11_USAGE_DEFAULT;
		VBDesc.ByteWidth = tMeshData.Vertices.size() * sizeof(GeometryGenerator::MeshData_PosNormalTex);
		VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VBDesc.CPUAccessFlags = 0;
		VBDesc.MiscFlags = 0;

		// Subresource Data
		D3D11_SUBRESOURCE_DATA VBInitData;
		VBInitData.pSysMem = tMeshData.Vertices.data();
		VBInitData.SysMemPitch = 0;
		VBInitData.SysMemSlicePitch = 0;

		// Create the vertex buffer.
		HRESULT hr = _DXStorage->_device->CreateBuffer(&VBDesc, &VBInitData, &_quadVB);

		// Buffer Description
		D3D11_BUFFER_DESC IBDesc;
		IBDesc.Usage = D3D11_USAGE_DEFAULT;
		IBDesc.ByteWidth = tMeshData.Indices.size() * sizeof(unsigned int);
		IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IBDesc.CPUAccessFlags = 0;
		IBDesc.MiscFlags = 0;

		// Subresource Data
		D3D11_SUBRESOURCE_DATA IBInitData;
		IBInitData.pSysMem = tMeshData.Indices.data();
		IBInitData.SysMemPitch = 0;
		IBInitData.SysMemSlicePitch = 0;

		// Create the Index buffer.
		hr = _DXStorage->_device->CreateBuffer(&IBDesc, &IBInitData, &_quadIB);
	}

	void OpaqueQuadRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void OpaqueQuadRenderPass::PassNextRequirements(ID3D11RenderTargetView**& rtvArray, unsigned int& rtvCount, 
		ID3D11ShaderResourceView**& srvArray, unsigned int& srvCount, ID3D11DepthStencilView*& dsv)
	{
		//하는거 없으면, 건드리지 말아야 한다.
	}

	void OpaqueQuadRenderPass::BindVertexIndexBuffer()
	{
		// Bind Buffers
		UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &_quadVB, &stride, &offset);
		_DXStorage->_deviceContext->IASetIndexBuffer(_quadIB, DXGI_FORMAT_R32_UINT, 0);
	}

	void OpaqueQuadRenderPass::BindMaterialIndexConstantBuffer()
	{
		//Mapped Subresource 메모리 클린.
		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HR(_DXStorage->_deviceContext->Map(_cbMatID, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

		//자신의 Data 시작 포인터 받아오기.
		UINT* data = reinterpret_cast<UINT*>(res.pData);

		//Material 부여.
		*(data) = _renderMaterial->GetID();

		_DXStorage->_deviceContext->Unmap(_cbMatID, 0);
	}

	void OpaqueQuadRenderPass::CreateMaterialIndexConstantBuffer()
	{
		//ID의 포인터를 받아온다.
		UINT* _cbData = &(_renderMaterial->GetID());
	
		//Constant Buffer 자체를 만드는 코드.
		int sizeCB = (((sizeof(unsigned int) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
		assert(sizeCB % 16 == 0);

		D3D11_BUFFER_DESC tDesc;
		tDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		tDesc.Usage = D3D11_USAGE_DYNAMIC;
		tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA tSubResource;
		tSubResource.pSysMem = _cbData;

		HR(_DXStorage->_device->CreateBuffer(&tDesc, &tSubResource, &(_cbMatID)));
	}

	

}