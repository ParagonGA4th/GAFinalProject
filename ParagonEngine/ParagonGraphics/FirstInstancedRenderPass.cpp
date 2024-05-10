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
		CreateShaders();
	}

	FirstInstancedRenderPass::~FirstInstancedRenderPass()
	{

	}

	void FirstInstancedRenderPass::Initialize()
	{
		//이는 Scene이 바뀔 때마다 다시 호출될 것이다.	

		//이제 나머지 Initialize.
		_lightmapCBuffer.reset(new ConstantBuffer<ConstantBufferDefine::cbLightmapCollection>());
		for (int i = 0; i < Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING; i++)
		{
			_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = SingleLightMapSet({ 1.f,1.f }, { 0.f,0.f }, 0);
		}
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
		RenderNormalInstanced(renderObjectList, camData);
		RenderCulledOppositeInstanced(renderObjectList, camData);
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

	void FirstInstancedRenderPass::RenderNormalInstanced(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);
		
		for (auto& [bModel, bBufferPairList] : tRenderObjectList->_instancedStaticList)
		{
			//우선적으로, ConstantBuffer부터 셋한다.
			assert(bBufferPairList->_instancedLightMapSetVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING);

			for (int i = 0; i < bBufferPairList->_instancedLightMapSetVec.size(); i++)
			{
				_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = bBufferPairList->_instancedLightMapSetVec.at(i);
			}
			
			_lightmapCBuffer->Update();

			//이제 PS CB Bind과정.
			_lightmapCBuffer->BindPS(5);

			//그리기.
			//bBufferPairList->_vb
			
		}
	}

	void FirstInstancedRenderPass::RenderCulledOppositeInstanced(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);

		for (auto& [bModel, bBufferPairList] : tRenderObjectList->_instancedCulledOppositeStaticList)
		{
			
		}
	}

}
