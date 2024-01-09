#include "FirstStaticRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"

namespace Pg::Graphics
{

	FirstStaticRenderPass::FirstStaticRenderPass()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	FirstStaticRenderPass::~FirstStaticRenderPass()
	{

	}

	void FirstStaticRenderPass::Initialize()
	{
		CreateD3DViews();
		CreateShaders();
	}

	void FirstStaticRenderPass::ReceiveRequiredElements(const std::vector<ID3D11RenderTargetView*>* rtvArray, unsigned int rtvCount,
		const std::vector<ID3D11ShaderResourceView*>* srvArray, unsigned int srvCount, ID3D11DepthStencilView* dsv)
	{
		//아무것도 받지 않는다.
	}

	void FirstStaticRenderPass::BindPass()
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

	void FirstStaticRenderPass::RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//모든 오브젝트 렌더링.
		for (auto& it : renderObjectList->_staticList)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				if (it.second->at(i).second->GetBaseRenderer()->GetActive())
				{
					it.second->at(i).second->First_UpdateConstantBuffers(camData);
					it.second->at(i).second->First_BindBuffers();
					it.second->at(i).second->First_Render();
					it.second->at(i).second->First_UnbindBuffers();
				}
			}
		}
	}

	void FirstStaticRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		_DXStorage->_deviceContext->OMSetRenderTargets(NullRTV.size(), nullptr, _gBufferDepthStencil->GetDSV());

		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void FirstStaticRenderPass::ExecuteNextRenderRequirements()
	{
		//FirstSkinnedRenderPass가 들어온다면, 이 호출부는 그 렌더 패스의 ExecuteNextRenderRequirements에 들어간다.
		
		//t0에, 5개의 SRV GBuffer 대응. (Depth 제외)
		_DXStorage->_deviceContext->PSSetShaderResources(0, 5, _SRVs.data());

		//t1에 Depth Buffer SRV 1개 대응.
		_DXStorage->_deviceContext->PSSetShaderResources(1, 1, &(_SRVs.back()));
	}

	void FirstStaticRenderPass::PassNextRequirements(std::vector<ID3D11RenderTargetView*>*& rtvArray, unsigned int& rtvCount, std::vector<ID3D11ShaderResourceView*>*& srvArray, unsigned int& srvCount, ID3D11DepthStencilView*& dsv)
	{
		//Execute 함수가 대신 실행해주었다.
	}

	void FirstStaticRenderPass::CreateD3DViews()
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

	void FirstStaticRenderPass::CreateShaders()
	{
		// 1st Pass
		_vs = std::make_unique<SystemVertexShader>(L"../Builds/x64/debug/FirstStatic_VS.cso", LayoutDefine::GetStatic1stLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(L"../Builds/x64/debug/FirstStage_PS.cso");
	}

	


}