#include "RenderCubemap.h"

#include "DX11Headers.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "AssetBasic2DLoader.h"
#include "GraphicsResourceManager.h"
#include "MathHelper.h"

#include "../ParagonData/CameraData.h"

#include <typeinfo>

namespace Pg::Graphics
{

	unsigned int RenderCubemap::_indexCount = 36;

	RenderCubemap::RenderCubemap(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		_DXStorage = LowDX11Storage::GetInstance();
		CreateConstantBuffers();
	}

	RenderCubemap::~RenderCubemap()
	{

	}

	void RenderCubemap::InternalLoad()
	{
		//중복되었는지 찾는 것은 외부에서 이루어질 일!
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Loader::AssetBasic2DLoader;

		AssetBasic2DLoader* t2DLoader = GraphicsResourceManager::Instance()->GetBasic2DLoader();
		t2DLoader->LoadCubemap(_filePath, this);
	}

	void RenderCubemap::InternalUnload()
	{
		//
	}

	void RenderCubemap::UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{
		DirectX::XMFLOAT4X4 tWorldTM;
		DirectX::XMMATRIX tWorldTMMat = DirectX::XMMatrixIdentity();

		DirectX::XMFLOAT4X4 tViewTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
		DirectX::XMMATRIX tViewTMMat = DirectX::XMLoadFloat4x4(&tViewTM);

		DirectX::XMFLOAT4X4 tProjTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
		DirectX::XMMATRIX tProjTMMat = DirectX::XMLoadFloat4x4(&tProjTM);

		DirectX::XMFLOAT3 tCameraPosition = Helper::MathHelper::PG2XM_FLOAT3(camData->_position);
		DirectX::XMVECTOR tCameraPositionVec = DirectX::XMLoadFloat3(&tCameraPosition);
		DirectX::XMMATRIX tCameraPositionMat = DirectX::XMMatrixTranslationFromVector(tCameraPositionVec);

		DirectX::XMStoreFloat4x4(&(_cbData.worldMatrix), DirectX::XMMatrixMultiply(tWorldTMMat, tCameraPositionMat));
		DirectX::XMStoreFloat4x4(&(_cbData.viewProjMatrix), DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));
	}

	void RenderCubemap::BindConstantBuffers()
	{
		_DXStorage->_deviceContext->VSSetConstantBuffers(0, 1, &_cBuffer);
	}

	void RenderCubemap::BindAdditionalResources()
	{
		_DXStorage->_deviceContext->PSSetShaderResources(0, 1, &_srv);
		_DXStorage->_deviceContext->PSSetSamplers(0, 1, &(_DXStorage->_defaultSamplerState));
	}

	void RenderCubemap::Render()
	{
		UINT stride = sizeof(LayoutDefine::VinCubemap);
		UINT offset = 0;

		LowDX11Storage::GetInstance()->_deviceContext->IASetVertexBuffers(0, 1, &_VB, &stride, &offset);
		LowDX11Storage::GetInstance()->_deviceContext->IASetIndexBuffer(_IB, DXGI_FORMAT_R32_UINT, 0);


		_DXStorage->_deviceContext->DrawIndexed(_indexCount, 0, 0);
	}

	void RenderCubemap::UnbindAdditionalResources()
	{

	}

	void RenderCubemap::UnbindConstantBuffers()
	{
		_DXStorage->_deviceContext->VSSetConstantBuffers(0, 1, &_nullBuffer);
	}

	void RenderCubemap::CreateConstantBuffers()
	{
		int sizeCB = (((sizeof(RenderCubemap::CB) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
		assert(sizeCB % 16 == 0);
		D3D11_BUFFER_DESC tCBufferDesc;
		tCBufferDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		tCBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		tCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tCBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tCBufferDesc.MiscFlags = 0;

		_cbSubresData.pSysMem = reinterpret_cast<const void*>(&_cbData);

		HR(_DXStorage->_device->CreateBuffer(&tCBufferDesc, &_cbSubresData, &_cBuffer));
	}
}





