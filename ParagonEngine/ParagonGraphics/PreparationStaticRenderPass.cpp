#include "PreparationStaticRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"

#include <algorithm> 
namespace Pg::Graphics
{

	PreparationStaticRenderPass::PreparationStaticRenderPass() : _rtBindArray(), _rtNullBindArray()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	PreparationStaticRenderPass::~PreparationStaticRenderPass()
	{

	}

	void PreparationStaticRenderPass::Initialize()
	{
		CreateD3DViews();
		CreateShaders();
		CreateBuffers();
	}

	void PreparationStaticRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_quadSaveDSV = carrier._quadMainGDS->GetDSV();
		_quadSaveObjMatGBuffer = carrier._quadObjMatRT;
		
		//OMSetRenderTargets 바인딩을 위해 사용. 나머지 인덱스는 초기에 바인딩.
		_rtBindArray[0] = _quadSaveObjMatGBuffer->GetRTV();
	}

	void PreparationStaticRenderPass::BindPass()
	{
		//전체 RenderTargetView 클리어.
		for (int i = 0; i < 4; i++)
		{
			_DXStorage->_deviceContext->ClearRenderTargetView(_rtBindArray[i], _DXStorage->_backgroundColor);
		}

		_DXStorage->_deviceContext->OMSetRenderTargets(_rtBindArray.size(), _rtBindArray.data(), _quadSaveDSV);
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
					it.second->at(i).second->ObjMat_Render();
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
		_DXStorage->_deviceContext->OMSetRenderTargets(_rtNullBindArray.size(), _rtNullBindArray.data(), nullptr);

		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void PreparationStaticRenderPass::ExecuteNextRenderRequirements()
	{
		//만약 Skinned가 들어온다면, 이 코드는 ObjMatSkinnedRenderPass로 가야 한다.
		//당연히 GBuffer-DepthStencil 역시 옮겨받아야 하고.

		//t3에, ObjMat GBuffer가 들어간다. 대응. (Depth 제외)
		_DXStorage->_deviceContext->PSSetShaderResources(3, 1, &(_quadSaveObjMatGBuffer->GetSRV()));

		//t12-14 - internalPBRTextures Bind
		_DXStorage->_deviceContext->PSSetShaderResources(12, 1, &(_albedoAmbiBuffer->GetSRV()));
		_DXStorage->_deviceContext->PSSetShaderResources(13, 1, &(_normalRoughBuffer->GetSRV()));
		_DXStorage->_deviceContext->PSSetShaderResources(14, 1, &(_specularMetalBuffer->GetSRV()));

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

	void PreparationStaticRenderPass::CreateD3DViews()
	{
		//DepthStencil은 MainQuadDepthStencil이다. (Skinned도 마찬가지)
		//OpaqueQuad 시리즈가 가능한 이유는,
		//Rendering은 Main Render Target에 함에도 DepthStencil을 자체적으로 생성해서 쓰기 때문 (기존의 값이 영향을 주지 않음)

		_albedoAmbiBuffer = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		_normalRoughBuffer = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		_specularMetalBuffer = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT);

		//일단 값을 OMSetRenderTargets를 위해 설정.
		_rtBindArray[0] = nullptr;
		_rtBindArray[1] = _albedoAmbiBuffer->GetRTV();
		_rtBindArray[2] = _normalRoughBuffer->GetRTV();
		_rtBindArray[3] = _specularMetalBuffer->GetRTV();

		//NullRTV Array를 위해, nullptr 채우기!
		std::fill(_rtNullBindArray.begin(), _rtNullBindArray.end(), nullptr);
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