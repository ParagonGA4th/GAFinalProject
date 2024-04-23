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

		//여기서는 RenderTarget Bind만. 어차피 Skinned에서는 들어갈 때 설정안해주고
		//끝나면 Clear해준다.
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
					//만약 Transform의 Scale 중 1/3개 (홀수)가 음수일 경우,
					//Rasterizer를 CullFront로 설정!
					//Static에 한정.
					bool isOddMinus = it.second->at(i).first->_transform.IsScaleOddMinus();

					if (isOddMinus)
					{
						_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidFrontfaceCullingState);
					}

					it.second->at(i).second->ObjMat_UpdateConstantBuffers(camData);
					it.second->at(i).second->ObjMat_BindBuffers();
					it.second->at(i).second->ObjMat_Render(nullptr);
					it.second->at(i).second->ObjMat_UnbindBuffers();

					if (isOddMinus)
					{
						_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidState);
					}
				}
			}
		}
	}

	void PreparationStaticRenderPass::UnbindPass()
	{
		// Unbind RenderTarget -> Skinned에서 해줄것이다. 여기서는 필요 X.
		//더 이상 값을 설정하지 않을 때 이런 식으로 할당 해제해주면 된다.
		//_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierStorage->_pbrNullBindArray.size(), _d3dCarrierStorage->_pbrNullBindArray.data(), nullptr);

		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void PreparationStaticRenderPass::ExecuteNextRenderRequirements()
	{
	
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
}