#include "Forward3DRenderer.h"

#include "DX11Headers.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"

#include "MathHelper.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"

#include "../ParagonData/CameraData.h"
#include "../ParagonData/AssetDefines.h"

#include "Grid.h"
#include "Axis.h"
#include "RenderCubemap.h"
#include "RenderObjectCubemapList.h"

#include <algorithm>
#include <cassert>

namespace Pg::Graphics
{
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	Pg::Graphics::Forward3DRenderer::Forward3DRenderer()
		: _DXStorage(LowDX11Storage::GetInstance()), _DXLogic(LowDX11Logic::GetInstance())
	{

	}

	void Pg::Graphics::Forward3DRenderer::Initialize()
	{
		CreateSystemVertexShaders();
		InitializePrimitiveWireframeObjects();
	}

	void Pg::Graphics::Forward3DRenderer::Render(RenderObjectCubemapList* cubeMapList, unsigned int cubeMapIndex, Pg::Data::CameraData* camData)
	{
		//일단은 Render Target을 Main으로 설정.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), _DXStorage->_depthStencilView);

		RenderWireframePrimitives(camData);
		RenderCubemapWithIndex(camData, cubeMapList, cubeMapIndex);
	}

	void Forward3DRenderer::RenderCubemapWithIndex(Pg::Data::CameraData* camData, RenderObjectCubemapList* cubeMapList, unsigned int cubemapIndex)
	{
		//Layout, Topology, Shader, RS
		_cubemapVS->Bind();
		_cubemapPS->Bind();

		//실제 Cubemap 렌더.
		cubeMapList->_list.at(cubemapIndex)->BindAdditionalResources();
		///231214 Last.
		//cubeMapList->_list.at(cubemapIndex)->Bind();
		cubeMapList->_list.at(cubemapIndex)->UpdateConstantBuffers(camData);
		cubeMapList->_list.at(cubemapIndex)->BindConstantBuffers();
		cubeMapList->_list.at(cubemapIndex)->Render();
		cubeMapList->_list.at(cubemapIndex)->UnbindConstantBuffers();

		//Shaders Unbind.
		_cubemapVS->Unbind();
		_cubemapPS->Unbind();
	}

	void Forward3DRenderer::CreateSystemVertexShaders()
	{
		_primitiveVS = std::make_unique<SystemVertexShader>(L"../Builds/x64/debug/PrimitiveVS.cso", LayoutDefine::GetWireframePrimitiveLayout(),
			LowDX11Storage::GetInstance()->_wireframeState, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		_primitivePS = std::make_unique<SystemPixelShader>(L"../Builds/x64/debug/PrimitivePS.cso");

		_cubemapVS = std::make_unique<SystemVertexShader>(L"../Builds/x64/debug/CubemapVS.cso", LayoutDefine::GetCubemapLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_cubemapPS = std::make_unique<SystemPixelShader>(L"../Builds/x64/debug/CubemapPS.cso");
	}

	void Forward3DRenderer::RenderWireframePrimitives(Pg::Data::CameraData* camData)
	{
		//Layout, Topology, Shader, RS
		_primitiveVS->Bind();
		_primitivePS->Bind();

		for (auto& it : _primObjectList)
		{
			it->UpdateConstantBuffers(camData);
			it->BindConstantBuffers();
			it->Render();
			it->UnbindConstantBuffers();
		}

		_primitiveVS->Unbind();
		_primitivePS->Unbind();
	}

	void Forward3DRenderer::InitializePrimitiveWireframeObjects()
	{
		// Primitive RenderObject 투입 + Initialize();
		_primObjectList.push_back(std::make_unique<Grid>());
		_primObjectList.push_back(std::make_unique<Axis>());

		//일괄적으로 BuildBuffers 수행.
		for (auto& it : _primObjectList)
		{
			it->BuildBuffers();
		}
	}
}


