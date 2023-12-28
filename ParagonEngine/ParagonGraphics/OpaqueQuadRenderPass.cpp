#include "OpaqueQuadRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GeometryGenerator.h"
#include "RenderVertexShader.h"
#include "RenderPixelShader.h"
#include "RenderMaterial.h"

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

	}

	void OpaqueQuadRenderPass::BindPass()
	{
		BindVertexIndexBuffer();
		_renderMaterial->Bind();
		//BindShaders(); 바뀔 수 있어야 한다. -> 어떤 셰이더가 들어오고, 처리하는지.
	}

	void OpaqueQuadRenderPass::RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		for (auto& it : *(renderObjectList->_list.at(_renderMaterial->GetFilePath())))
		{
			it.second->UpdateConstantBuffers(camData);
			it.second->BindBuffers();
			it.second->Render();
			it.second->UnbindBuffers();
		}
	}

	void OpaqueQuadRenderPass::UnbindPass()
	{
		_renderMaterial->Unbind();
	}

	void OpaqueQuadRenderPass::SetupNextRequirements()
	{

	}

	void OpaqueQuadRenderPass::GenerateQuadBuffer()
	{
		GeometryGenerator tGeometryGenerator;
		GeometryGenerator::MeshData_PosNormalTex tMeshData;
		tGeometryGenerator.GenerateFullscreenQuad(tMeshData);

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

	void OpaqueQuadRenderPass::BindVertexIndexBuffer()
	{
		// Bind Buffers
		UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		UINT offset = 0;
		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &_quadVB, &stride, &offset);
		_DXStorage->_deviceContext->IASetIndexBuffer(_quadIB, DXGI_FORMAT_R32_UINT, 0);
	}



}