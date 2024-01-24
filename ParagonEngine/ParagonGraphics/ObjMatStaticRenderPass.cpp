#include "ObjMatStaticRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"

namespace Pg::Graphics
{

	ObjMatStaticRenderPass::ObjMatStaticRenderPass()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	ObjMatStaticRenderPass::~ObjMatStaticRenderPass()
	{

	}

	void ObjMatStaticRenderPass::Initialize()
	{
		CreateD3DViews();
		CreateShaders();
	}

	void ObjMatStaticRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_quadSaveDSV = carrier._quadMainGDS->GetDSV();
		_quadSaveObjMatGBuffer = carrier._quadObjMatRT;
	}

	void ObjMatStaticRenderPass::BindPass()
	{
		_DXStorage->_deviceContext->ClearRenderTargetView(_quadSaveObjMatGBuffer->GetRTV(), _DXStorage->_backgroundColor);

		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_quadSaveObjMatGBuffer->GetRTV()), _quadSaveDSV);
		//_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_gBufferRender->GetRTV()), _DXStorage->_depthStencilView);

		_vs->Bind();
		_ps->Bind();
	}

	void ObjMatStaticRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
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
	}

	void ObjMatStaticRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		//더 이상 값을 설정하지 않을 때 이런 식으로 할당 해제해주면 된다.
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void ObjMatStaticRenderPass::ExecuteNextRenderRequirements()
	{
		//만약 Skinned가 들어온다면, 이 코드는 ObjMatSkinnedRenderPass로 가야 한다.
		//당연히 GBuffer-DepthStencil 역시 옮겨받아야 하고.

		//t3에, ObjMat GBuffer가 들어간다. 대응. (Depth 제외)
		_DXStorage->_deviceContext->PSSetShaderResources(3, 1, &(_quadSaveObjMatGBuffer->GetSRV()));
	}

	void ObjMatStaticRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

	void ObjMatStaticRenderPass::CreateD3DViews()
	{
		//DepthStencil은 MainQuadDepthStencil이다. (Skinned도 마찬가지)
		//OpaqueQuad 시리즈가 가능한 이유는,
		//Rendering은 Main Render Target에 함에도 DepthStencil을 자체적으로 생성해서 쓰기 때문 (기존의 값이 영향을 주지 않음)
	}

	void ObjMatStaticRenderPass::CreateShaders()
	{
		//ObjMatStatic 용도 셰이더 갖고 오기.
		_vs = std::make_unique<SystemVertexShader>(L"../Builds/x64/Debug/Individual_PerObjMatStaticVS.cso", LayoutDefine::GetPerObjMatStaticLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(L"../Builds/x64/Debug/Individual_PerObjMatPS.cso");
	}

}