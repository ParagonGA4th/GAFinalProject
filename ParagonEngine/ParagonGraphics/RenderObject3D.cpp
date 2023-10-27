#include "RenderObject3D.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonUtil/ResourceHelper.h"
#include "LowDX11Storage.h"
#include "MathHelper.h"
#include "ConstantBufferDefine.h"

#include "DX11Headers.h"

#include "dxtk/DDSTextureLoader.h"

#include "Asset3DModelData.h"


#include "VertexShader.h"
#include "PixelShader.h"

#include <cassert>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using Pg::Data::Enums::eAssetDefine;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Util::Helper::ResourceHelper;

	RenderObject3D::RenderObject3D(Pg::Data::BaseRenderer* baseRenderer) : RenderObjectBase(baseRenderer)
	{
		_constantBufferStruct = new ConstantBufferDefine::cbPerObjectBase;

		//Device / DevCon 받아오기.
		_device = LowDX11Storage::GetInstance()->_device;
		_devCon = LowDX11Storage::GetInstance()->_deviceContext;
	}

	RenderObject3D::~RenderObject3D()
	{

	}

	//void RenderObject3D::UpdateConstantBuffer(Pg::Data::CameraData* camData)
	//{
	//	Pg::Math::PGFLOAT4X4 tWorldMat = GetBaseRenderer()->_object->_transform.GetWorldTM();
	//	DirectX::XMFLOAT4X4 tWorldMatFF = MathHelper::PG2XM_FLOAT4X4(tWorldMat);
	//	DirectX::XMMATRIX tWorldMatFFMat = DirectX::XMLoadFloat4x4(&tWorldMatFF);

	//	//0.01 스케일링 적용.
	//	tWorldMatFFMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f), tWorldMatFFMat);

	//	DirectX::XMMATRIX tWorldInvTransposeMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, tWorldMatFFMat));

	//	//기반 Struct채우기.
	//	_constantBufferStruct->gCBuf_World = tWorldMatFFMat;
	//	_constantBufferStruct->gCBuf_WorldInvTranspose = tWorldInvTransposeMat;

	//	DirectX::XMFLOAT4X4 tView = MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
	//	DirectX::XMMATRIX tViewMat = DirectX::XMLoadFloat4x4(&tView);

	//	DirectX::XMFLOAT4X4 tProj = MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
	//	DirectX::XMMATRIX tProjMat = DirectX::XMLoadFloat4x4(&tProj);

	//	DirectX::XMMATRIX tWVP = DirectX::XMMatrixMultiply(tWorldMatFFMat, DirectX::XMMatrixMultiply(tViewMat, tProjMat));
	//	_constantBufferStruct->gCBuf_WorldViewProj = tWVP;
	//	_constantBufferStruct->gCBuf_CameraPositionW = MathHelper::PG2XM_FLOAT3(camData->_position);

	//	//Updating Subresource
	//	_devCon->UpdateSubresource(_constantBuffer, 0, NULL, _constantBufferStruct, 0, 0);
	//}


	void RenderObject3D::Render(Pg::Data::CameraData* camData)
	{

	}

	void RenderObject3D::Initialize()
	{
		//BuildBuffers();
		BindBuffers();
	}

	void RenderObject3D::BindShaders()
	{
		_vertexShader->Bind();
		_pixelShader->Bind();

		// Bind Constant Buffers
		for (auto& cb : _vertexShader->_constantBuffers)
		{
			cb->UpdateAndBind();
		}

		// Bind Shader Resources
		_DXStorage->_deviceContext->PSSetShaderResources(0, 1, &SRV);

		//
		_DXStorage->_deviceContext->PSSetSamplers(0, 1, &_samplerState);
	}

	void RenderObject3D::UnbindShaders()
	{
		_vertexShader->UnBind();
		_pixelShader->UnBind();
	}

	void RenderObject3D::BindInputLayout()
	{
		_DXStorage->_deviceContext->IASetInputLayout(_vertexShader->_inputLayout);
		_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void RenderObject3D::UnbindInputLayout()
	{
		_DXStorage->_deviceContext->IASetInputLayout(nullptr);
	}

	void RenderObject3D::AssignVertexShader(VertexShader* shader)
	{
		_vertexShader = shader;
	}

	void RenderObject3D::AssignPixelShader(PixelShader* shader)
	{
		_pixelShader = shader;
	}

	Pg::Graphics::VertexShader* RenderObject3D::GetVertexShader()
	{
		return _vertexShader;
	}

	Pg::Graphics::PixelShader* RenderObject3D::GetPixelShader()
	{
		return _pixelShader;
	}

	void RenderObject3D::SetTexture(std::wstring filepath)
	{
		ID3D11Resource* Texture = nullptr;

		HRESULT hr = DirectX::CreateDDSTextureFromFile(_DXStorage->_device, filepath.c_str(), &Texture, &SRV);

		_DXStorage->_deviceContext->PSSetShaderResources(0, 1, &SRV);

		D3D11_SAMPLER_DESC sd;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		sd.MipLODBias = 0.0f;
		sd.MaxAnisotropy = 1;

		_DXStorage->_device->CreateSamplerState(&sd, &_samplerState);
		_DXStorage->_deviceContext->PSSetSamplers(0, 1, &_samplerState);
	}

}