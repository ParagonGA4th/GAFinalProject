#include "DeferredRenderer.h"

#include "TestCube.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"

#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"

#include "LayoutDefine.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/LightType.h"

#include "RenderObjectBase.h"

#include "RenderObject3D.h"
#include "RenderObject3DList.h"
#include "RenderObjectLightList.h"

#include "ConstantBufferDefine.h"

#include "GeometryGenerator.h"

Pg::Graphics::DeferredRenderer::DeferredRenderer()
{

}

void Pg::Graphics::DeferredRenderer::Initialize()
{
	_DXStorage = LowDX11Storage::GetInstance();

	_gBufferRenderList.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBufferRenderList.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBufferRenderList.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBufferRenderList.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBufferRenderList.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));

	_gBufferDepthStencil = std::make_unique<GBufferDepthStencil>(DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_D32_FLOAT_S8X24_UINT);

	//FirstStage_PS에서 Binding될 Render Target들.
	for (auto& e : _gBufferRenderList)
	{
		_RTVs.emplace_back(e->GetRTV());
	}

	//SecondStage들에서 Binding될 SRV들. (GBufferRender, ~5/6)
	for (auto& e : _gBufferRenderList)
	{
		_SRVs.emplace_back(e->GetSRV());
	}

	//SecondStage들에서 Binding될 Depth SRV. (GBufferDepthStencil, 6/6)
	_SRVs.emplace_back(_gBufferDepthStencil->GetSRV());
	
	//지금까지 바인딩된 값만큼 RTV Null Array를 만들어준다.
	for (int i = 0; i < _gBufferRenderList.size() + 1; ++i)
	{
		NullRTV.emplace_back(nullptr);
	}

	//지금까지 바인딩된 값만큼 SRV Null Array를 만들어준다.
	for (int i = 0; i < _SRVs.size(); ++i)
	{
		NullSRV.emplace_back(nullptr);
	}

	BuildFullscreenQuad();

	// 1st Pass
	_firstVS = new SystemVertexShader(L"../Builds/x64/debug/FirstStatic_VS.cso", LayoutDefine::GetStatic1stLayout(),
		LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_firstPS = new SystemPixelShader(L"../Builds/x64/debug/FirstStage_PS.cso");

	//Lighting
	_lightingVS = new SystemVertexShader(L"../Builds/x64/debug/PhongVS.cso", LayoutDefine::GetDeferredQuadLayout(),
		LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_lightingPS = new SystemPixelShader(L"../Builds/x64/debug/PhongPS.cso");

	// 2nd Pass
	_secondVS = new SystemVertexShader(L"../Builds/x64/debug/SecondStage_VS.cso", LayoutDefine::GetDeferredQuadLayout(),
		LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_secondPS = new SystemPixelShader(L"../Builds/x64/debug/SecondStage_PS.cso");
}

void Pg::Graphics::DeferredRenderer::BeginRender()
{
	// Set Depth Stencil State
	_DXStorage->_deviceContext->OMSetDepthStencilState(_DXStorage->_depthStencilState, 0);

	_DXStorage->_deviceContext->ClearDepthStencilView(_DXStorage->_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
	ClearGBuffers();

	_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), _DXStorage->_depthStencilView);
}

void Pg::Graphics::DeferredRenderer::BindFirstPass()
{
	// Bind Shaders
	_firstVS->Bind();
	_firstPS->Bind();

	_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidState);
	_DXStorage->_deviceContext->PSSetShaderResources(0, 1, &NullSRV[0]);
	_DXStorage->_deviceContext->PSSetSamplers(0, 1, &_DXStorage->_defaultSamplerState);

	_DXStorage->_deviceContext->OMSetRenderTargets(_RTVs.size(), _RTVs.data(), _DXStorage->_depthStencilView);
}

void Pg::Graphics::DeferredRenderer::RenderFirstPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
{

	for (auto& it : renderObjectList->_list)
	{
		if (it.second->GetBaseRenderer()->GetActive())
		{
			it.second->UpdateConstantBuffers(camData);
			it.second->BindConstantBuffers();
			it.second->Render();
			it.second->UnbindConstantBuffers();
		}
	}

}

void Pg::Graphics::DeferredRenderer::UnbindFirstPass()
{
	// Unbind Shaders
	_firstVS->Unbind();
	_firstPS->Unbind();

	_DXStorage->_deviceContext->OMSetRenderTargets(_RTVs.size(), NullRTV.data(), _DXStorage->_depthStencilView);
}

void Pg::Graphics::DeferredRenderer::BindSecondPass()
{
	// Bind Quad
	BindFullscreenQuad();

	// Bind Shaders
	_secondVS->Bind();
	_secondPS->Bind();

	// Bind Constant Buffers

	// Set Shader Resources to Sample
	_DXStorage->_deviceContext->VSSetShaderResources(0, _SRVs.size(), _SRVs.data());
	_DXStorage->_deviceContext->PSSetShaderResources(0, _SRVs.size(), _SRVs.data());

	// Set Sampler State
	_DXStorage->_deviceContext->PSSetSamplers(0, 1, &_DXStorage->_defaultSamplerState);

	_DXStorage->_deviceContext->ClearRenderTargetView(_DXStorage->_mainRTV, _DXStorage->_backgroundColor);
	// Render to Main Render Target
	_DXStorage->_deviceContext->OMSetRenderTargets(1, &_DXStorage->_mainRTV, _DXStorage->_depthStencilView);
	_DXStorage->_deviceContext->OMSetDepthStencilState(_DXStorage->_2ndPassDepthStencilState, 0);
}

void Pg::Graphics::DeferredRenderer::RenderSecondPass()
{
	//Quad를 그린다.
	_DXStorage->_deviceContext->DrawIndexed(6, 0, 0);

	//_DXStorage->_deviceContext->OMSetBlendState(nullptr, NULL, 0xffffffff);
	//_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), _DXStorage->_depthStencilView);
}

void Pg::Graphics::DeferredRenderer::UnbindSecondPass()
{
	_secondVS->Unbind();
	_secondPS->Unbind();

	UnbindConstantBuffers(_lightingCBs);

	_DXStorage->_deviceContext->VSSetShaderResources(0, _SRVs.size(), NullSRV.data());
	_DXStorage->_deviceContext->PSSetShaderResources(0, _SRVs.size(), NullSRV.data());

	_DXStorage->_deviceContext->OMSetRenderTargets(_RTVs.size(), NullRTV.data(), _DXStorage->_depthStencilView);
	_DXStorage->_deviceContext->OMSetDepthStencilState(_DXStorage->_depthStencilState, 0);
}

void Pg::Graphics::DeferredRenderer::BuildFullscreenQuad()
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
	HRESULT hr = _DXStorage->_device->CreateBuffer(&VBDesc, &VBInitData, &_VB);

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
	hr = _DXStorage->_device->CreateBuffer(&IBDesc, &IBInitData, &_IB);
}

void Pg::Graphics::DeferredRenderer::ClearGBuffers()
{
	for (auto& e : _RTVs)
	{
		_DXStorage->_deviceContext->ClearRenderTargetView(e, _DXStorage->_backgroundColor);
	}
}

void Pg::Graphics::DeferredRenderer::BindLightingPass()
{
	auto LightingBuffer = _gBuffers[6];
	auto LightingBufferSRV = LightingBuffer->GetSRV();
	auto LightingBufferRTV = LightingBuffer->GetRTV();

	// Bind Quad
	BindFullscreenQuad();

	// Bind Shaders
	// TODO: 라이팅 모델에 따라 쉐이더가 바뀔 수 있어야 한다.
	// ex) Lit / Unlit / Blinn-Phong / PBR ,,, 
	_lightingVS->Bind();
	_lightingPS->Bind();

	BindConstantBuffers(_lightingCBs);

	// Set Shader Resources to Sample
	_DXStorage->_deviceContext->VSSetShaderResources(0, _SRVs.size() - 1, _SRVs.data());
	_DXStorage->_deviceContext->PSSetShaderResources(0, _SRVs.size() - 1, _SRVs.data());

	// Set Sampler State
	_DXStorage->_deviceContext->PSSetSamplers(0, 1, &_DXStorage->_defaultSamplerState);

	_DXStorage->_deviceContext->ClearRenderTargetView(LightingBufferRTV, _DXStorage->_backgroundColor);
	// Render to Main Render Target
	_DXStorage->_deviceContext->OMSetRenderTargets(1, &LightingBufferRTV, _DXStorage->_depthStencilView);
	_DXStorage->_deviceContext->OMSetDepthStencilState(_DXStorage->_2ndPassDepthStencilState, 0);

}

void Pg::Graphics::DeferredRenderer::UnbindLightingPass()
{
	//UnbindConstantBuffers(_lightingCBs);

	_lightingVS->Unbind();
	_lightingPS->Unbind();

	_DXStorage->_deviceContext->OMSetRenderTargets(_RTVs.size(), NullRTV.data(), _DXStorage->_depthStencilView);
}

void Pg::Graphics::DeferredRenderer::RenderLight(RenderObjectLightList* lightList, Pg::Data::CameraData* camData)
{
	// 조명 정보를 담고 있는 상수버퍼를 조립하고 업데이트
	lightList->Update(camData);
	UpdateConstantBuffers(_lightingCBs);

	// 조명을 연산한다
	_DXStorage->_deviceContext->DrawIndexed(6, 0, 0);
}

void Pg::Graphics::DeferredRenderer::UpdateConstantBuffers(const std::vector< ConstantBufferBase*>& _constantBuffers)
{
	for (int i = 0; i < _constantBuffers.size(); ++i)
	{
		_constantBuffers[i]->Update(i);
	}
}

void Pg::Graphics::DeferredRenderer::BindConstantBuffers(const std::vector< ConstantBufferBase*>& _constantBuffers)
{
	for (int i = 0; i < _constantBuffers.size(); ++i)
	{
		_constantBuffers[i]->Bind(i);
	}
}

void Pg::Graphics::DeferredRenderer::UnbindConstantBuffers(const std::vector< ConstantBufferBase*>& _constantBuffers)
{
	for (int i = 0; i < _constantBuffers.size(); ++i)
	{
		_constantBuffers[i]->Unbind(i);
	}
}

void Pg::Graphics::DeferredRenderer::BindFullscreenQuad()
{
	// Bind Buffers
	UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
	UINT offset = 0;
	_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &_VB, &stride, &offset);
	_DXStorage->_deviceContext->IASetIndexBuffer(_IB, DXGI_FORMAT_R32_UINT, 0);
}
