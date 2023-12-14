#include "Forward3DRenderer.h"

#include "DX11Headers.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "MathHelper.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"

#include "../ParagonData/CameraData.h"
#include "../ParagonData/AssetDefines.h"

#include "Grid.h"
#include "Axis.h"
#include "WireframeRenderObject.h"
#include "RenderCubemap.h"

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
		InitializeCubemaps();


	}

	void Pg::Graphics::Forward3DRenderer::Render(Pg::Data::CameraData* camData)
	{
		//일단은 Render Target을 Main으로 설정.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), _DXStorage->_depthStencilView);

		RenderWireframePrimitives(camData);
		RenderCubemapWithIndex(camData, 0);
	}

	void Forward3DRenderer::RenderCubemapWithIndex(Pg::Data::CameraData* camData, unsigned int cubemapIndex)
	{
		//Layout, Topology, Shader, RS
		_cubemapVS->Bind();
		_cubemapPS->Bind();

		//실제 Cubemap 렌더.
		_cubeMapList.at(cubemapIndex)->UpdateConstantBuffers(camData);
		_cubeMapList.at(cubemapIndex)->BindConstantBuffers();
		_cubeMapList.at(cubemapIndex)->BindAdditionalResources();
		_cubeMapList.at(cubemapIndex)->Render();
		_cubeMapList.at(cubemapIndex)->UnbindConstantBuffers();

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

	void Forward3DRenderer::InitializeCubemaps()
	{
		//Index : 1 추가.
		{
			//Cubemap 데이터를 받기.
			auto tCubemapData = GraphicsResourceManager::Instance()->GetResource("../Resources/Textures/room.dds", eAssetDefine::_CUBEMAP);
			_cubeMapList.push_back(static_cast<RenderCubemap*>(tCubemapData.get()));
		}


	}
}


