#include "FinalRenderPass.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GeometryGenerator.h"

namespace Pg::Graphics
{
	FinalRenderPass::FinalRenderPass()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	FinalRenderPass::~FinalRenderPass()
	{

	}

	void FinalRenderPass::Initialize()
	{
		CreateShaders();

	}

	void FinalRenderPass::BindPass()
	{
		//이미 MainRenderTarget 관련된 Clear 등 상호작용은 ParagonRenderer의 시작에서 실행되었다.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &_DXStorage->_mainRTV, _DXStorage->_depthStencilView);

		//Quad의 Vertex, Index 바인딩.
		BindVertexIndexBuffer();

		// 셰이더 바인딩.
		_vs->Bind();
		_ps->Bind();
	}

	void FinalRenderPass::RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//RenderPass로 받아야 하지만, 
		//Quad 전체를 MainRenderTarget으로 옮기기만 하는 얘는 상관 없다.
		
		//PSSetShaderResources


	}

	void FinalRenderPass::UnbindPass()
	{
		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void FinalRenderPass::CreateShaders()
	{
		// 1st Pass
		_vs = std::make_unique<SystemVertexShader>(L"../Builds/x64/debug/FinalStage_VS.cso", LayoutDefine::GetDeferredQuadLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(L"../Builds/x64/debug/FinalStage_PS.cso");
	}

	void FinalRenderPass::CreateVertexIndexBuffer()
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

	void FinalRenderPass::BindVertexIndexBuffer()
	{
		// Bind Buffers
		UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &_quadVB, &stride, &offset);
		_DXStorage->_deviceContext->IASetIndexBuffer(_quadIB, DXGI_FORMAT_R32_UINT, 0);
	}

	void FinalRenderPass::ReceiveRequiredElements(void* place1, void* place2, void* place3, void* place4)
	{

	}

	void FinalRenderPass::PassOnNextRequirements(void** place1, void** place2, void** place3, void** place4)
	{
		//마지막 Render Pass, 세팅할 이유가 없다.
	}

}