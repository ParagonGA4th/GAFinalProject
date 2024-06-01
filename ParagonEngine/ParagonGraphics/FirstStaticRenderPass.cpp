#include "FirstStaticRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemInterfacedVertexShader.h"
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

		_switchableViewProjCBuffer.reset(new ConstantBuffer<ConstantBufferDefine::cbSwitchableViewProj>());
	}

	void FirstStaticRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
{
		//D3DCarrier의 포인터만 잠시 저장해놓는다.
		_d3dCarrierTempStorage = &carrier;
	}

	void FirstStaticRenderPass::BindPass()
	{
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
				//Culling 로직 추가.
				if (it.second->at(i).second->GetBaseRenderer()->GetActive() && (!(it.second->at(i).second->GetIsCulledFromRendering())))
				{
					//만약 Transform의 Scale 중 1/3개 (홀수)가 음수일 경우,
					//Rasterizer를 CullFront로 설정!
					//Static에 한정.
					bool isOddMinus = it.second->at(i).first->_transform.IsScaleOddMinus();
					
					if (isOddMinus)
					{
						_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidFrontfaceCullingState);
					}

					_switchableViewProjCBuffer->GetDataStruct()->_viewProj = Pg::Math::PG2XM_MATRIX4X4(camData->_viewMatrix * camData->_projMatrix);
					_switchableViewProjCBuffer->Update();
					_switchableViewProjCBuffer->BindVS(1);

					it.second->at(i).second->First_UpdateConstantBuffers(camData);
					it.second->at(i).second->First_BindBuffers();
					it.second->at(i).second->First_Render(nullptr);

					//_vs->Unbind();
					//_vs->Bind(1);
					//이때 ViewProj 업데이트해야.
					_switchableViewProjCBuffer->UnbindVS(1);
					_switchableViewProjCBuffer->GetDataStruct()->_viewProj = _d3dCarrierTempStorage->_mainLightPerspectiveViewProjMatrix;
					_switchableViewProjCBuffer->Update();
					_switchableViewProjCBuffer->BindVS(1);

					_ps->Unbind();
					_depthRecordOnlyPS->Bind();
					_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);
					//_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
					_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_d3dCarrierTempStorage->_mainLightGBufRT->GetRTV()), _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
					
					it.second->at(i).second->First_Render(nullptr);

					it.second->at(i).second->First_UnbindBuffers();

					
					if (isOddMinus)
					{
						_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidState);
					}
					_switchableViewProjCBuffer->UnbindVS(1);
					_depthRecordOnlyPS->Unbind();
				}
			}
		}
	}

	void FirstStaticRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		// Unbind Shaders
		_vs->Unbind();
		//_ps->Unbind();
	}

	void FirstStaticRenderPass::ExecuteNextRenderRequirements()
	{
		
	}

	void FirstStaticRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void FirstStaticRenderPass::CreateShaders()
	{
		using Pg::Util::Helper::ResourceHelper;
		//// 1st Pass
		//_vs = std::make_unique<SystemInterfacedVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_STATIC_VS_DIRECTORY), LayoutDefine::GetStatic1stLayout(),
		//	LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, std::string("g_ViewProjGetter"), std::initializer_list<std::string>{ std::string("CCameraViewProjGet"), std::string("CMainLightViewProjGet")});
		_vs = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_STATIC_VS_DIRECTORY), LayoutDefine::GetStatic1stLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_STAGE_PS_DIRECTORY));
		_depthRecordOnlyPS = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_DEPTH_ONLY_STAGE_PS_DIRECTORY));
	}



}