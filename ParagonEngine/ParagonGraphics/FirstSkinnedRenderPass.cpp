#include "FirstSkinnedRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemInterfacedVertexShader.h"
#include "RenderTexture2D.h"
#include "RenderCubemap.h"
#include "SystemPixelShader.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/ParagonDefines.h"

#include <algorithm>  

namespace Pg::Graphics
{

	FirstSkinnedRenderPass::FirstSkinnedRenderPass(const Pg::Data::Enums::eEditorMode* const editorMode) : _editorMode(editorMode)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	FirstSkinnedRenderPass::~FirstSkinnedRenderPass()
	{

	}

	void FirstSkinnedRenderPass::Initialize()
	{
		CreateShaders();

		_switchableViewProjCBuffer.reset(new ConstantBuffer<ConstantBufferDefine::cbSwitchableViewProj>());
	}

	void FirstSkinnedRenderPass::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

	void FirstSkinnedRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		//D3DCarrier�� �����͸� ��� �����س��´�.
		_d3dCarrierTempStorage = &carrier;
	}

	void FirstSkinnedRenderPass::BindPass()
	{
		//�̹� Clear�� �� �ܰ迡�� �ߴ�. (Static) ->�׷���, ����Ÿ�ٸ� ���ε�.

		// ���̴� ���ε�.
		_vs->Bind();
	}

	void FirstSkinnedRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);

		//��� ������Ʈ ������.
		for (auto& it : tRenderObjectList->_skinnedList)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				if (it.second->at(i).second->GetBaseRenderer()->GetActive() && (!(it.second->at(i).second->GetIsCulledFromRendering())))
				{
					_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(),
						_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

					_ps->Bind();

					_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_defaultViewport));
					_switchableViewProjCBuffer->GetDataStruct()->_viewProj = Pg::Math::PG2XM_MATRIX4X4(camData->_viewMatrix * camData->_projMatrix);
					_switchableViewProjCBuffer->Update();
					_switchableViewProjCBuffer->BindVS(1);

					it.second->at(i).second->UpdateAnimationInfo(&_deltaTimeStorage, _editorMode);
					it.second->at(i).second->First_UpdateConstantBuffers(camData);
					it.second->at(i).second->First_BindBuffers();
					it.second->at(i).second->First_Render(&_deltaTimeStorage);

					//_vs->Unbind();
					//_vs->Bind(1);
					///�̶� ViewProj �ٲ����!
					_switchableViewProjCBuffer->UnbindVS(1);
					_switchableViewProjCBuffer->GetDataStruct()->_viewProj = _d3dCarrierTempStorage->_mainLightPerspectiveViewProjMatrix;
					_switchableViewProjCBuffer->Update();
					_switchableViewProjCBuffer->BindVS(1);
					_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_shadowMapViewport));

					_ps->Unbind();
					_depthRecordOnlyPS->Bind();
					_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);
					//_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
					//_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_d3dCarrierTempStorage->_mainLightGBufRT->GetRTV()), _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
					ID3D11RenderTargetView* tEmptyRenderTargets[1] = { _d3dCarrierTempStorage->_mainLightGBufRT->GetRTV() };
					_DXStorage->_deviceContext->OMSetRenderTargets(1, tEmptyRenderTargets, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());

					it.second->at(i).second->First_Render(&_deltaTimeStorage);

					it.second->at(i).second->First_UnbindBuffers();

					_switchableViewProjCBuffer->UnbindVS(1);

					_depthRecordOnlyPS->Unbind();
				}
			}
		}
	}

	void FirstSkinnedRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		// Unbind Shaders
		_vs->Unbind();
		//_ps->Unbind();
	}

	void FirstSkinnedRenderPass::ExecuteNextRenderRequirements()
	{
		//������ �ۿ��� �Ѵ�.
		
		//���⼭ �ϴ� ����, ���� ������ �ٽ� Viewport�� ��������.
		_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_defaultViewport));
	}

	void FirstSkinnedRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void FirstSkinnedRenderPass::CreateShaders()
	{
		//�ϴ��� ������ �������� ���� ������ Vertex Shader �ϳ��� �ʿ��Կ���
		//Static���� Unbind�� �߱⿡, Pixel Shader�� �ٽ� ���ε��Ѵ�.
		using Pg::Util::Helper::ResourceHelper;
		using namespace Pg::Defines;
		//ResourceHelper::IfReleaseChangeDebugTextW(

		// 1st Pass
		//_vs = std::make_unique<SystemInterfacedVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(FIRST_SKINNED_VS_DIRECTORY), 
		//	LayoutDefine::GetSkinned1stLayout(),
		//	LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 
		//	std::string("g_ViewProjGetter"), std::initializer_list<std::string>{ std::string("CCameraViewProjGet"), std::string("CMainLightViewProjGet")});

		_vs = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(FIRST_SKINNED_VS_DIRECTORY),
			LayoutDefine::GetSkinned1stLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(FIRST_STAGE_PS_DIRECTORY));
		_depthRecordOnlyPS = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_DEPTH_ONLY_STAGE_PS_DIRECTORY));

	}
	

}
