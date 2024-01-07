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

	}

	void ObjMatStaticRenderPass::RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{

	}

	void ObjMatStaticRenderPass::UnbindPass()
	{

	}

	void ObjMatStaticRenderPass::ExecuteNextRenderRequirements()
	{

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