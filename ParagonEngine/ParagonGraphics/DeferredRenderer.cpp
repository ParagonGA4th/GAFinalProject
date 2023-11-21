#include "DeferredRenderer.h"

#include "TestCube.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "GBuffer.h"
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

	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R16G16B16A16_TYPELESS, DXGI_FORMAT_R16G16B16A16_FLOAT));
	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
	_gBuffers.emplace_back(new GBuffer(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT)); // phong lighting results

	for (auto& e : _gBuffers)
	{
		_RTVs.emplace_back(e->GetRTV());
	}

	for (auto& e : _gBuffers)
	{
		_SRVs.emplace_back(e->GetSRV());
	}

	for (int i = 0; i < _gBuffers.size(); ++i)
	{
		NullRTV.emplace_back(nullptr);
	}

	for (int i = 0; i < _gBuffers.size(); ++i)
	{
		NullSRV.emplace_back(nullptr);
	}

	BuildFullscreenQuad();

	// 1st Pass
	_firstVS = new VertexShader(Pg::Data::Enums::eAssetDefine::_RENDERSHADER, "../Builds/x64/debug/FirstStatic_VS.cso");
	_firstVS->_inputLayout = LayoutDefine::GetStatic1stLayout();
	_firstPS = new PixelShader(Pg::Data::Enums::eAssetDefine::_RENDERSHADER, "../Builds/x64/debug/FirstStage_PS.cso");

	_lightingVS = new VertexShader(Pg::Data::Enums::eAssetDefine::_RENDERSHADER, "../Builds/x64/debug/PhongVS.cso");
	_lightingVS->_inputLayout = LayoutDefine::Get2ndLayout();
	_lightingPS = new PixelShader(Pg::Data::Enums::eAssetDefine::_RENDERSHADER, "../Builds/x64/debug/PhongPS.cso");

	// 2nd Pass
	_secondVS = new VertexShader(Pg::Data::Enums::eAssetDefine::_RENDERSHADER, "../Builds/x64/debug/SecondStage_VS.cso");
	_secondVS->_inputLayout = LayoutDefine::Get2ndLayout();
	_secondPS = new PixelShader(Pg::Data::Enums::eAssetDefine::_RENDERSHADER, "../Builds/x64/debug/SecondStage_PS.cso");
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
	renderObjectList->_renderedObjectCount = 0;

	for (auto& it : renderObjectList->_list)
	{
		if (it.second->GetBaseRenderer()->GetActive())
		{
			if (IsInFrustum(it.second.get(), camData))
			{
				it.second->UpdateConstantBuffers(camData);
				it.second->BindConstantBuffers();
				it.second->Render();
				it.second->UnbindConstantBuffers();
				renderObjectList->_renderedObjectCount++;
			}
		}


	}

}

void Pg::Graphics::DeferredRenderer::UnbindFirstPass()
{
	// Unbind Shaders
	_firstVS->UnBind();
	_firstPS->UnBind();

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
	_secondVS->UnBind();
	_secondPS->UnBind();

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
	IBDesc.ByteWidth = tMeshData.Indices.size() * sizeof(int);
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

	_lightingVS->UnBind();
	_lightingPS->UnBind();

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

void Pg::Graphics::DeferredRenderer::UpdateConstantBuffers(std::vector< ConstantBufferBase*> _constantBuffers)
{
	for (int i = 0; i < _constantBuffers.size(); ++i)
	{
		_constantBuffers[i]->Update(i);
	}
}

void Pg::Graphics::DeferredRenderer::BindConstantBuffers(std::vector< ConstantBufferBase*> _constantBuffers)
{
	for (int i = 0; i < _constantBuffers.size(); ++i)
	{
		_constantBuffers[i]->Bind(i);
	}
}

void Pg::Graphics::DeferredRenderer::UnbindConstantBuffers(std::vector< ConstantBufferBase*> _constantBuffers)
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

bool Pg::Graphics::DeferredRenderer::IsInFrustum(RenderObject3D* object, Pg::Data::CameraData* camData)
{
	Pg::Data::PGFLOAT4 p;
	Pg::Data::PGFLOAT4 n;

	float result;
	Pg::Data::PGFLOAT4 q;


	q = Pg::Data::PGFLOAT4(object->GetBaseRenderer()->_object->GetComponent<Pg::Data::Transform>()->GetPosition(), 1.0f);
	q = Pg::Math::PGFloat4MultiplyMatrix(q, camData->_viewMatrix);

	// Near Plane
	p = { 0.0f, 0.0f, camData->_nearZ, 0.0f};
	n = { 0.0f, 0.0f, 1.0f, 0.0f };

	result = n.x * q.x + n.y * q.y + n.z * q.z - (n.x * p.x + n.y * p.y + n.z * p.z);

	if (result < 0)
	{
		return false;
	}

	// Far Plane
	p = { 0.0f, 0.0f, camData->_farZ, 0.0f };
	n = { 0.0f, 0.0f, -1.0f, 0.0f };

	result = n.x * q.x + n.y * q.y + n.z * q.z - (n.x * p.x + n.y * p.y + n.z * p.z);

	if (result < 0)
	{
		return false;
	}

	// Right Plane
	p = {camData->_nearWindowHeight * camData->_aspect / 2, 0.0f, camData->_nearZ, 0.0f };
	n = { camData->_nearWindowHeight * camData->_nearZ / 2, 0.0f, camData->_aspect * camData->_nearWindowHeight * camData->_nearWindowHeight / 4, 0.0f};
	
	result = n.x * q.x + n.y * q.y + n.z * q.z - (n.x * p.x + n.y * p.y + n.z * p.z);

	if (result < 0)
	{
		return false;
	}

	// Left Plane
	p = {(-1.0f) * camData->_nearWindowHeight * camData->_aspect / 2, 0.0f, camData->_nearZ, 0.0f };
	n = {(-1.0f) * camData->_nearWindowHeight * camData->_nearZ / 2, 0.0f, camData->_aspect * camData->_nearWindowHeight * camData->_nearWindowHeight / 4, 0.0f };
	
	result = n.x * q.x + n.y * q.y + n.z * q.z - (n.x * p.x + n.y * p.y + n.z * p.z);

	if (result < 0)
	{
		return false;
	}

	// Up Plane
	p = {0.0f , camData->_nearWindowHeight / 2, camData->_nearZ, 0.0f };
	n = {0.0f, camData->_nearWindowHeight * camData->_aspect * camData->_nearZ / -2, camData->_nearWindowHeight * camData->_aspect * camData->_nearWindowHeight / 4, 0.0f};

	result = n.x * q.x + n.y * q.y + n.z * q.z - (n.x * p.x + n.y * p.y + n.z * p.z);

	if (result < 0)
	{
		return false;
	}

	// Down Plane
	p = {0.0f , (-1.0f) * camData->_nearWindowHeight / 2, camData->_nearZ, 0.0f };
	n = { 0.0f, (-1.0f) * camData->_nearWindowHeight * camData->_aspect * camData->_nearZ / -2, camData->_nearWindowHeight * camData->_aspect * camData->_nearWindowHeight / 4, 0.0f };

	result = n.x * q.x + n.y * q.y + n.z * q.z - (n.x * p.x + n.y * p.y + n.z * p.z);

	if (result < 0)
	{
		return false;
	}

	// 프러스텀 안에 존재한다면 true를 반환
	return true;
}
