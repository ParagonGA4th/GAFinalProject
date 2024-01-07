#include "ObjMatRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"

namespace Pg::Graphics
{

	ObjMatRenderPass::ObjMatRenderPass()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	ObjMatRenderPass::~ObjMatRenderPass()
	{

	}

	void ObjMatRenderPass::Initialize()
	{
		CreateD3DViews();
		CreateShaders();
	}

	void ObjMatRenderPass::ReceiveRequiredElements(const std::vector<ID3D11RenderTargetView*>* rtvArray, unsigned int rtvCount, const std::vector<ID3D11ShaderResourceView*>* srvArray, unsigned int srvCount, ID3D11DepthStencilView* dsv)
	{

	}

	void ObjMatRenderPass::BindPass()
	{

	}

	void ObjMatRenderPass::RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{

	}

	void ObjMatRenderPass::UnbindPass()
	{

	}

	void ObjMatRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void ObjMatRenderPass::PassNextRequirements(std::vector<ID3D11RenderTargetView*>*& rtvArray, unsigned int& rtvCount, std::vector<ID3D11ShaderResourceView*>*& srvArray, unsigned int& srvCount, ID3D11DepthStencilView*& dsv)
	{

	}

	void ObjMatRenderPass::CreateD3DViews()
	{

	}

	void ObjMatRenderPass::CreateShaders()
	{

	}

}