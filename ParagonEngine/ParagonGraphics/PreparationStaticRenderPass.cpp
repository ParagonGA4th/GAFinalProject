#include "PreparationStaticRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "RenderTexture2D.h"
#include "RenderCubemap.h"
#include "SystemPixelShader.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/ParagonDefines.h"

#include <algorithm> 
namespace Pg::Graphics
{

	PreparationStaticRenderPass::PreparationStaticRenderPass() 
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	PreparationStaticRenderPass::~PreparationStaticRenderPass()
	{

	}

	void PreparationStaticRenderPass::Initialize()
	{
		CreateShaders();
		CreateBuffers();
		
	}

	void PreparationStaticRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_d3dCarrierStorage = &carrier;

		//_quadSaveDSV = _d3dCarrierStorage->_quadMainGDS->GetDSV();
		//_quadSaveObjMatGBuffer = _d3dCarrierStorage->_quadObjMatRT;
		
		//OMSetRenderTargets 바인딩을 위해 사용. 나머지 인덱스는 초기에 바인딩.
		//_rtBindArray[0] = _quadSaveObjMatGBuffer->GetRTV();
	}

	void PreparationStaticRenderPass::BindPass()
	{
		//전체 RenderTargetView 클리어.
		for (int i = 0; i < 4; i++)
		{
			_DXStorage->_deviceContext->ClearRenderTargetView(_d3dCarrierStorage->_pbrBindArray[i], _DXStorage->_backgroundColor);
		}

		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierStorage->_pbrBindArray.size(), _d3dCarrierStorage->_pbrBindArray.data(), _d3dCarrierStorage->_quadMainGDS->GetDSV());
		//_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_gBufferRender->GetRTV()), _DXStorage->_depthStencilView);

		_vs->Bind();
		_ps->Bind();
	}

	void PreparationStaticRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
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
					it.second->at(i).second->ObjMat_UpdateConstantBuffers(camData);
					it.second->at(i).second->ObjMat_BindBuffers();
					it.second->at(i).second->ObjMat_Render(nullptr);
					it.second->at(i).second->ObjMat_UnbindBuffers();
				}
			}
		}

		//Camera Data 변수 저장.
		_savedCamData = camData;
	}

	void PreparationStaticRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		//더 이상 값을 설정하지 않을 때 이런 식으로 할당 해제해주면 된다.
		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierStorage->_pbrNullBindArray.size(), _d3dCarrierStorage->_pbrNullBindArray.data(), nullptr);

		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void PreparationStaticRenderPass::ExecuteNextRenderRequirements()
	{
		//Constant Buffer (SceneInfo) 업데이트.
		_cbSceneInfo->GetDataStruct()->gCBuf_ViewMatrix = PG2XM_MATRIX4X4(_savedCamData->_viewMatrix);
		_cbSceneInfo->GetDataStruct()->gCBuf_ProjMatrix = PG2XM_MATRIX4X4(_savedCamData->_projMatrix);
		_cbSceneInfo->GetDataStruct()->gCBuf_EyePosition = PG2XM_FLOAT3(_savedCamData->_position);
		//업데이트.
		_cbSceneInfo->Update();
		//SceneInfo Constant Buffer Bind -> 나중에 CBCarrier뭐 이런 클래스 있어야 할 것이다.
		_cbSceneInfo->BindPS(4);
	}

	void PreparationStaticRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void PreparationStaticRenderPass::CreateShaders()
	{
		//ObjMatStatic 용도 셰이더 갖고 오기.
		_vs = std::make_unique<SystemVertexShader>(L"../Builds/x64/Debug/Individual_PerObjMatStaticVS.cso", LayoutDefine::GetPerObjMatStaticLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(L"../Builds/x64/Debug/Individual_PerObjMatPS.cso");
	}

	void PreparationStaticRenderPass::CreateBuffers()
	{
		_cbSceneInfo = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbSceneInfo>>();
	}

	

}