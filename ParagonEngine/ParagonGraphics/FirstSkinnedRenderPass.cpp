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
	}

	void FirstSkinnedRenderPass::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

	void FirstSkinnedRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		//D3DCarrier의 포인터만 잠시 저장해놓는다.
		_d3dCarrierTempStorage = &carrier;
	}

	void FirstSkinnedRenderPass::BindPass()
	{
		//이미 Clear는 전 단계에서 했다. (Static) ->그러니, 렌더타겟만 바인딩.
		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(),
			_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

		// 셰이더 바인딩.
		_vs->Bind(0);
		_ps->Bind();
	}

	void FirstSkinnedRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);

		//모든 오브젝트 렌더링.
		for (auto& it : tRenderObjectList->_skinnedList)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				if (it.second->at(i).second->GetBaseRenderer()->GetActive() && (!(it.second->at(i).second->GetIsCulledFromRendering())))
				{
					it.second->at(i).second->UpdateAnimationInfo(&_deltaTimeStorage, _editorMode);
					it.second->at(i).second->First_UpdateConstantBuffers(camData);
					it.second->at(i).second->First_BindBuffers();
					it.second->at(i).second->First_Render(&_deltaTimeStorage);

					_vs->Unbind();
					_vs->Bind(1);
					_ps->Unbind();
					_depthRecordOnlyPS->Bind();
					_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);
					_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());

					it.second->at(i).second->First_Render(&_deltaTimeStorage);

					it.second->at(i).second->First_UnbindBuffers();

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
		_ps->Unbind();
	}

	void FirstSkinnedRenderPass::ExecuteNextRenderRequirements()
	{
		//이제는 밖에서 한다.
	}

	void FirstSkinnedRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void FirstSkinnedRenderPass::CreateShaders()
	{
		//일단은 구조의 용이함을 위해 당장은 Vertex Shader 하나만 필요함에도
		//Static에서 Unbind를 했기에, Pixel Shader를 다시 바인드한다.
		using Pg::Util::Helper::ResourceHelper;
		using namespace Pg::Defines;
		//ResourceHelper::IfReleaseChangeDebugTextW(

		// 1st Pass
		_vs = std::make_unique<SystemInterfacedVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(FIRST_SKINNED_VS_DIRECTORY), 
			LayoutDefine::GetSkinned1stLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 
			std::string("g_ViewProjGetter"), std::initializer_list<std::string>{ std::string("CCameraViewProjGet"), std::string("CMainLightViewProjGet")});
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(FIRST_STAGE_PS_DIRECTORY));
		_depthRecordOnlyPS = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_DEPTH_ONLY_STAGE_PS_DIRECTORY));

	}
	

}
