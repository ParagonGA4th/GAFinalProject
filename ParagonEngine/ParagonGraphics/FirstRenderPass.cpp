#include "FirstRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"

namespace Pg::Graphics
{

	FirstRenderPass::FirstRenderPass()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	FirstRenderPass::~FirstRenderPass()
	{

	}

	void FirstRenderPass::Initialize()
	{
		CreateD3DViews();
		CreateShaders();
	}

	void FirstRenderPass::ReceiveRequiredElements(void* place1, void* place2, void* place3, void* place4)
	{

	}

	void FirstRenderPass::BindPass()
	{
		//자체적인 DSV Clear, Depth Stencil State 리셋, OMSetRenderTargets.
		_DXStorage->_deviceContext->ClearDepthStencilView(_gBufferDepthStencil->GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetDepthStencilState(_gBufferDepthStencil->GetDSState(), 0);

		for (auto& e : _RTVs)
		{
			_DXStorage->_deviceContext->ClearRenderTargetView(e, _DXStorage->_backgroundColor);
		}

		_DXStorage->_deviceContext->OMSetRenderTargets(_RTVs.size(), _RTVs.data(), _gBufferDepthStencil->GetDSV());

		// 셰이더 바인딩.
		_vs->Bind();
		_ps->Bind();
	}

	void FirstRenderPass::RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//모든 오브젝트 렌더링.
		for (auto& it : renderObjectList->_list)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				if (it.second->at(i).second->GetBaseRenderer()->GetActive())
				{
					it.second->at(i).second->UpdateConstantBuffers(camData);
					it.second->at(i).second->BindBuffers();
					it.second->at(i).second->Render();
					it.second->at(i).second->UnbindBuffers();
				}
			}
		}
	}

	void FirstRenderPass::UnbindPass()
	{
		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void FirstRenderPass::PassOnNextRequirements(void** place1, void** place2, void** place3, void** place4)
	{

	}

	void FirstRenderPass::CreateD3DViews()
	{
		//RT0
		_gBufferRenderList.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT1
		_gBufferRenderList.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT2
		_gBufferRenderList.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT3
		_gBufferRenderList.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT4
		_gBufferRenderList.emplace_back(std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT));
		//RT5 (Depth)
		_gBufferDepthStencil = std::make_unique<GBufferDepthStencil>(DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_D32_FLOAT_S8X24_UINT);

		//FirstStage_PS에서 Binding될 Render Target들.
		//Depth는 자동 연동 (DepthStencil 바인딩 공간 별도 존재)
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
	}

	void FirstRenderPass::CreateShaders()
	{
		// 1st Pass
		_vs = std::make_unique<SystemVertexShader>(L"../Builds/x64/debug/FirstStatic_VS.cso", LayoutDefine::GetStatic1stLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(L"../Builds/x64/debug/FirstStage_PS.cso");
	}
	

}