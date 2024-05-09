#include "FirstInstancedRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"

namespace Pg::Graphics
{
	FirstInstancedRenderPass::FirstInstancedRenderPass() : _DXStorage(LowDX11Storage::GetInstance())
	{

	}

	FirstInstancedRenderPass::~FirstInstancedRenderPass()
	{

	}

	void FirstInstancedRenderPass::Initialize()
	{
		CreateShaders();
	}

	void FirstInstancedRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_d3dCarrierTempStorage = &carrier;
	}

	void FirstInstancedRenderPass::BindPass()
	{
		//자체적인 DSV Clear, Depth Stencil State 리셋, OMSetRenderTargets.
		_DXStorage->_deviceContext->ClearDepthStencilView(_d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetDepthStencilState(_d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSState(), 0);

		for (auto& e : _d3dCarrierTempStorage->_gBufRequiredRTVArray)
		{
			_DXStorage->_deviceContext->ClearRenderTargetView(e, _DXStorage->_backgroundColor);
		}

		//자신을 위한 Bind.
		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(),
			_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

		// 셰이더 바인딩.
		_vs->Bind();
		_ps->Bind();

	}

	void FirstInstancedRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);

		//

	}

	void FirstInstancedRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);

		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void FirstInstancedRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void FirstInstancedRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void FirstInstancedRenderPass::CreateShaders()
	{
		using Pg::Util::Helper::ResourceHelper;

		// Instanced Pass
		_vs = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_VS_DIRECTORY), LayoutDefine::GetInstanced1stLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_STAGE_PS_DIRECTORY));
	}

	void FirstInstancedRenderPass::SendToGPUInstanceData_Lightmap(void* renderObjectList, const Pg::Data::Scene* const newScene)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);

		//이미 개별적으로 외적 Culling되는 애들은 넣어놓았기에, SV_InstanceID가 어긋나지는 않을 것이다.
		//개별적으로 Model 중심으로 정렬되는 코드. 
		//LoadTRSBuffer를 중심으로 SV_InstanceID Increment : 
		//그러니, 모델 별로 InstanceID는 독립적이라고 보는게 맞다.
		//Transform은 매프레임마다 보내주는 것이 옳다.

		//씬 단위에서는 그냥 Lightmapping 데이터만 보내주는게 맞다. 
		//지금 이 함수는 씬이 바뀔 때만 작동하니.
		//1. Unity Lightmap Data를 바꿔야 한다. -> 오브젝트 이름 포함하게!
		//2. 오브젝트 매칭해야지. -> 이를 InstanceMesh에 보관.
		//3. Runtime에 검사하자. Lightmap 리스트, SoundSystem이 하는 것처럼 쫙 훑어서 bool값으로 이미 다른 Scene과 연동되었는지를 검사.
		// 그 다음에, 없을 경우에는 런타임에, ParseScene 파트에다가 연동하는 것처럼 ㄱㄱ! 
		//4. 



	}

}
