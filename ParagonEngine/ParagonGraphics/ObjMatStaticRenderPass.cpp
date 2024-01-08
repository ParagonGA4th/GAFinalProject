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

	void ObjMatStaticRenderPass::ReceiveRequiredElements(const std::vector<ID3D11RenderTargetView*>* rtvArray, unsigned int rtvCount, const std::vector<ID3D11ShaderResourceView*>* srvArray, unsigned int srvCount, ID3D11DepthStencilView* dsv)
	{

	}

	void ObjMatStaticRenderPass::BindPass()
	{
		//자체적인 DSV Clear, Depth Stencil State 리셋, OMSetRenderTargets.
		_DXStorage->_deviceContext->ClearDepthStencilView(_gBufferDepthStencil->GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetDepthStencilState(_gBufferDepthStencil->GetDSState(), 0);

		_DXStorage->_deviceContext->ClearRenderTargetView(_gBufferRender->GetRTV(), _DXStorage->_backgroundColor);

		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_gBufferRender->GetRTV()), _gBufferDepthStencil->GetDSV());

		_vs->Bind();
		_ps->Bind(); 
	}

	void ObjMatStaticRenderPass::RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//모든 오브젝트 렌더링.
		for (auto& it : renderObjectList->_list)
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
		_DXStorage->_deviceContext->OMSetRenderTargets(1, nullptr, _gBufferDepthStencil->GetDSV());

		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}		 

	void ObjMatStaticRenderPass::ExecuteNextRenderRequirements()
	{
		//만약 Skinned가 들어온다면, 이 코드는 ObjMatSkinnedRenderPass로 가야 한다.
		//당연히 GBuffer-DepthStencil 역시 옮겨받아야 하고.
		
		//t3에, ObjMat GBuffer가 들어간다. 대응. (Depth 제외)
		_DXStorage->_deviceContext->PSSetShaderResources(3, 1, &(_gBufferRender->GetSRV()));
	}

	void ObjMatStaticRenderPass::PassNextRequirements(std::vector<ID3D11RenderTargetView*>*& rtvArray, unsigned int& rtvCount, std::vector<ID3D11ShaderResourceView*>*& srvArray, unsigned int& srvCount, ID3D11DepthStencilView*& dsv)
	{

	}

	void ObjMatStaticRenderPass::CreateD3DViews()
	{
		//ObjMat RenderTarget
		_gBufferRender = std::make_unique<GBufferRender>(DXGI_FORMAT_R32G32_TYPELESS, DXGI_FORMAT_R32G32_UINT);

		//ObjMat 용도로 쓸 DepthStencil.
		_gBufferDepthStencil = std::make_unique<GBufferDepthStencil>(DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_D32_FLOAT_S8X24_UINT);
	}

	void ObjMatStaticRenderPass::CreateShaders()
	{
		//ObjMatStatic 용도 셰이더 갖고 오기.
		_vs = std::make_unique<SystemVertexShader>(L"../Builds/x64/Debug/Individual_PerObjMatStaticVS.cso", LayoutDefine::GetPerObjMatStaticLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(L"../Builds/x64/Debug/Individual_PerObjMatPS.cso");
	}

}