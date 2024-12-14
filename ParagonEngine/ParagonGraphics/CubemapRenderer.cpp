#include "CubemapRenderer.h"

#include "DX11Headers.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"

#include "MathHelper.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"

#include "../ParagonData/CameraData.h"
#include "../ParagonData/AssetDefines.h"

#include "RenderCubemap.h"
#include "RenderObjectCubemapList.h"

#include "../ParagonData/ParagonDefines.h"
#include "../ParagonHelper/ResourceHelper.h"

#include <algorithm>
#include <cassert>

namespace Pg::Graphics
{
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	Pg::Graphics::CubemapRenderer::CubemapRenderer(D3DCarrier* d3dCarrier) 
		: BaseSpecificRenderer(d3dCarrier), _DXStorage(LowDX11Storage::GetInstance()), _DXLogic(LowDX11Logic::GetInstance())
	{

	}

	void Pg::Graphics::CubemapRenderer::Initialize()
	{
		CreateSystemVertexShaders();
	}

	void CubemapRenderer::SetupRenderPasses()
	{

	}

	void CubemapRenderer::RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData)
	{
		Render((RenderObjectCubemapList*)renderObjectList, camData);
	}

	void Pg::Graphics::CubemapRenderer::Render(RenderObjectCubemapList* cubeMapList, Pg::Data::CameraData* camData)
	{
		//������ Quad�� �ִ� ������ ����.
		//_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_carrier->_quadMainRT->GetRTV()), _carrier->_quadMainGDS->GetDSV());
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_carrier->_quadMainRT->GetRTV()), _carrier->_gBufRequiredInfoDSV->GetDSV());
		//_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_carrier->_quadMainRT->GetRTV()), nullptr);

		RenderCubemapWithIndex(camData, cubeMapList, _internalCubemapList);
	}

	void CubemapRenderer::ConfirmCarrierData()
	{

	}

	void CubemapRenderer::RenderCubemapWithIndex(Pg::Data::CameraData* camData, RenderObjectCubemapList* cubeMapList, unsigned int cubemapIndex)
	{
		//Layout, Topology, Shader, RS
		_cubemapVS->Bind();
		_cubemapPS->Bind();

		//���� Cubemap ����.
		cubeMapList->_list.at(cubemapIndex)->BindAdditionalResources();
		cubeMapList->_list.at(cubemapIndex)->UpdateConstantBuffers(camData);
		cubeMapList->_list.at(cubemapIndex)->BindConstantBuffers();
		cubeMapList->_list.at(cubemapIndex)->Render();
		cubeMapList->_list.at(cubemapIndex)->UnbindConstantBuffers();

		//Shaders Unbind.
		_cubemapVS->Unbind();
		_cubemapPS->Unbind();
	}

	void CubemapRenderer::CreateSystemVertexShaders()
	{
		using Pg::Util::Helper::ResourceHelper;
		using namespace Pg::Defines;
		//ResourceHelper::IfReleaseChangeDebugTextW(

		_cubemapVS = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(CUBEMAP_VS_DIRECTORY), LayoutDefine::GetCubemapLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_cubemapPS = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(CUBEMAP_PS_DIRECTORY));
	}

	


}


