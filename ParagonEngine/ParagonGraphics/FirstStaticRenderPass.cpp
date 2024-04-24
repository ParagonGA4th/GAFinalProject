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
		CreateShaders();
	}

	void FirstStaticRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
{
		//D3DCarrier의 포인터만 잠시 저장해놓는다.
		_d3dCarrierTempStorage = &carrier;
	}

	void FirstStaticRenderPass::BindPass()
	{
		//자체적인 DSV Clear, Depth Stencil State 리셋, OMSetRenderTargets.
		_DXStorage->_deviceContext->ClearDepthStencilView(_d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetDepthStencilState(_d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSState(), 0);

		for (auto& e : _d3dCarrierTempStorage->_gBufRequiredRTVArray)
		{
			_DXStorage->_deviceContext->ClearRenderTargetView(e, _DXStorage->_backgroundColor);
		}

		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), 
			_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

		// 셰이더 바인딩.
		_vs->Bind();
		_ps->Bind();
	}

	void FirstStaticRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);

		//모든 오브젝트 렌더링.
		for (auto& it : tRenderObjectList->_staticList)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				if (it.second->at(i).second->GetBaseRenderer()->GetActive())
				{
					//만약 Transform의 Scale 중 1/3개 (홀수)가 음수일 경우,
					//Rasterizer를 CullFront로 설정!
					//Static에 한정.
					bool isOddMinus = it.second->at(i).first->_transform.IsScaleOddMinus();
					
					if (isOddMinus)
					{
						_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidFrontfaceCullingState);
					}

					it.second->at(i).second->First_UpdateConstantBuffers(camData);
					it.second->at(i).second->First_BindBuffers();
					it.second->at(i).second->First_Render(nullptr);
					it.second->at(i).second->First_UnbindBuffers();

					if (isOddMinus)
					{
						_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidState);
					}
				}
			}
		}
	}

	void FirstStaticRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);

		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void FirstStaticRenderPass::ExecuteNextRenderRequirements()
	{
		
	}

	void FirstStaticRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void FirstStaticRenderPass::CreateShaders()
	{
		// 1st Pass
		_vs = std::make_unique<SystemVertexShader>(L"../Builds/x64/debug/FirstStatic_VS.cso", LayoutDefine::GetStatic1stLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(L"../Builds/x64/debug/FirstStage_PS.cso");
	}



}