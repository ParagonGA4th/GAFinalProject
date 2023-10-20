#include "ParagonRenderer.h"

#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceHelper.h"
#include "Grid.h"
#include "Axis.h"
#include "Cubemap.h"
#include "TestCube.h"
#include "DeferredRenderer.h"

#include "LayoutDefine.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/RendererChangeList.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonUtil/Log.h"

#include <utility>
#include <singleton-cpp/singleton.h>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;


	Grid* grid;
	Axis* axis;
	Cubemap* cubemap;

	ParagonRenderer::ParagonRenderer() :
		_DXStorage(LowDX11Storage::GetInstance()), _DXLogic(LowDX11Logic::GetInstance())
	{
		auto& tRendererChangeList = singleton<Pg::Data::RendererChangeList>();
		_rendererChangeList = &tRendererChangeList;

	}

	ParagonRenderer::~ParagonRenderer()
	{

	}

	void ParagonRenderer::Initialize()
	{		
		_deferredRenderer = new DeferredRenderer();
		_deferredRenderer->Initialize();

		D3D11_INPUT_ELEMENT_DESC HelperDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		LayoutDefine::GetStatic1stLayout();

		// TODO: TestBox와 Grid, Axis 모두 같은 InputLayout을 사용하고 있다...
		VertexShader* helperVS = new VertexShader(L"../Builds/x64/debug/VertexShader.cso", HelperDesc);
		PixelShader* helperPS = new PixelShader(L"../Builds/x64/debug/PixelShader.cso");

		// Grid
		grid = new Grid();
		grid->Initialize();

		// Axis
		axis = new Axis();
		axis->Initialize();

		//cubeVS->AssignConstantBuffer(&(cube->_cbData));
		helperVS->AssignConstantBuffer(&(grid->_cbData));

		grid->AssignVertexShader(helperVS);
		grid->AssignPixelShader(helperPS);

		axis->AssignVertexShader(helperVS);
		axis->AssignPixelShader(helperPS);

		//cube->AssignVertexShader(cubeVS);
		//cube->AssignPixelShader(cubePS);


		// Cubemap
		D3D11_INPUT_ELEMENT_DESC CubemapvertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		VertexShader* CubemapVS = new VertexShader(L"../Builds/x64/debug/CubemapVS.cso", CubemapvertexDesc);
		PixelShader* CubemapPS = new PixelShader(L"../Builds/x64/debug/CubemapPS.cso");

		cubemap = new Cubemap();
		cubemap->Initialize();

		CubemapVS->AssignConstantBuffer(&(cubemap->_cbData));
		cubemap->AssignVertexShader(CubemapVS);
		cubemap->AssignPixelShader(CubemapPS);
	}

	void ParagonRenderer::BeginRender()
	{
		_DXLogic->PrepareRenderTargets();
		_DXLogic->BindRenderTargets();
	}



	void ParagonRenderer::Render(Pg::Data::CameraData camData)
	{
		RenderDefaultObjects(camData);

		_DXStorage->_deviceContext->ClearDepthStencilView(_DXStorage->_DeferredDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		// 3D 오브젝트 렌더
		for (auto& it : _renderObject3DList)
		{
			if (it.second->_baseRenderer->GetActive())
			{
				_deferredRenderer->Render(*(it.first), camData);
				
			}
		}

		_deferredRenderer->ClearGBuffers();

		// 2D 오브젝트 렌더
		for (auto& it : _renderObject2DList)
		{
			if (it.second->_baseRenderer->GetActive())
			{
				//렌더.
			}
		}
	}

	void ParagonRenderer::EndRender()
	{
		_DXLogic->Present();
	}

	void ParagonRenderer::SyncComponentToGraphics()
	{
		using Pg::Data::GameObject;
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//새로 들어온 Renderer들 추가!
		for (auto it : _rendererChangeList->GetAddedRendererList())
		{
			GameObject* tGameObject = it->_object;

			if (GraphicsResourceHelper::IsRenderer3D(it->GetRendererTypeName()))
			{
				//3D
				auto tRes = _renderObject3DList.insert_or_assign(tGameObject, 
					std::make_unique<RenderObject3D>(it));

				if (!tRes.second)
				{
					//Assign
					PG_WARN("Already had a renderer (3D), but replaced with a new one.");
				}
			}
			else
			{
				//2D
				auto tRes = _renderObject2DList.insert_or_assign(tGameObject, 
					std::make_unique<RenderObject2D>(it));

				if (!tRes.second)
				{
					//Assign
					PG_WARN("Already had a renderer (2D), but replaced with a new one.");
				}
			}
		}

		//새로 들어온 Renderer를 삭제!
		for (auto it : _rendererChangeList->GetDeletedRendererList())
		{
			GameObject* tGameObject = it->_object;

			if (GraphicsResourceHelper::IsRenderer3D(it->GetRendererTypeName()))
			{
				//3D
				_renderObject3DList.erase(tGameObject);
			}
			else
			{
				//2D
				_renderObject2DList.erase(tGameObject);
			}
		}
	}

	void ParagonRenderer::OnNewSceneStart(Pg::Data::Scene* newScene)
	{
		//Scene을 파싱해서, 실제 렌더되어야 하는 Object를 연동한다.
		//나중에 같은 씬을 유지하는 중에 오브젝트들 중 하나의 렌더러가 꺼진다거나 
		//상황은 아직 유지 못함. 나중에 _rendererChangeList를 활용하면 된다!
		
		//기존의 직접적 RenderObject 리스트들 클리어.
		_renderObject2DList.clear();
		_renderObject3DList.clear();

		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//Scene에서 Renderer 정보를 가져오기 전, 
		//만약 렌더러 중 하나라면, Rendering Logic 연동.
		for (auto& tGameObject : newScene->GetObjectList())
		{
			for (auto& tComponentPair : tGameObject->GetComponentList())
			{
				if (GraphicsResourceHelper::IsRenderer3D(tComponentPair.first) != -1)
				{
					//만약 렌더러 파생 컴포넌트라면?
					Pg::Data::BaseRenderer* tBaseRenderer = static_cast<Pg::Data::BaseRenderer*>(tComponentPair.second);
					tBaseRenderer->SetRendererTypeName(tComponentPair.first);
				}
			}
		}

		//이제 실제 오브젝트 내부 RenderObject 연동.
		for (auto& tGameObject : newScene->GetObjectList())
		{
			//GameObject 딴.
			Pg::Data::BaseRenderer* tBaseRenderer = tGameObject->GetComponent<Pg::Data::BaseRenderer>();
			
			if (tBaseRenderer != nullptr)
			{
				//원래는 여기에 Active한지도 검사해야 한다.
				
				if (GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 1)
				{
					//3D
					auto tRes = _renderObject3DList.insert_or_assign(tGameObject, 
						std::make_unique<RenderObject3D>(tBaseRenderer));
				}
				else
				{
					//2D
					auto tRes = _renderObject2DList.insert_or_assign(tGameObject, 
						std::make_unique<RenderObject2D>(tBaseRenderer));
				}
			}
		}

		assert(true);
	}

	void ParagonRenderer::RenderDefaultObjects(Pg::Data::CameraData camData)
{
		///
		DirectX::XMFLOAT4X4 tWorldTM;
		DirectX::XMMATRIX tWorldTMMat = DirectX::XMMatrixIdentity();

		DirectX::XMFLOAT4X4 tViewTM = MathHelper::PG2XM_FLOAT4X4(camData._viewMatrix);
		DirectX::XMMATRIX tViewTMMat = DirectX::XMLoadFloat4x4(&tViewTM);

		DirectX::XMFLOAT4X4 tProjTM = MathHelper::PG2XM_FLOAT4X4(camData._projMatrix);
		DirectX::XMMATRIX tProjTMMat = DirectX::XMLoadFloat4x4(&tProjTM);

		DirectX::XMFLOAT3 tCameraPosition = MathHelper::PG2XM_FLOAT3(camData._position);
		DirectX::XMVECTOR tCameraPositionVec = DirectX::XMLoadFloat3(&tCameraPosition);
		DirectX::XMMATRIX tCameraPositionMat = DirectX::XMMatrixTranslationFromVector(tCameraPositionVec);

		float tCamDistance = 0.0f;
		DirectX::XMStoreFloat(&tCamDistance, DirectX::XMVector3Length(tCameraPositionVec));

		// Cubemap
		DirectX::XMStoreFloat4x4(&(cubemap->_cbData.worldMatrix), DirectX::XMMatrixMultiply(tWorldTMMat, tCameraPositionMat));
		DirectX::XMStoreFloat4x4(&(cubemap->_cbData.viewProjMatrix), DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));

		// Grid
		DirectX::XMStoreFloat4x4(&(grid->_cbData.worldMatrix), tWorldTMMat);
		DirectX::XMStoreFloat4x4(&(grid->_cbData.viewProjMatrix), DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));
		grid->SetGridSize(20.0f, 20.0f, 10, 10);

		// Axis
		DirectX::XMStoreFloat4x4(&(axis->_cbData.worldMatrix), tWorldTMMat);
		DirectX::XMStoreFloat4x4(&(axis->_cbData.viewProjMatrix), DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));

		// 렌더
		cubemap->Draw();
		grid->Draw();
		axis->Draw();
	}

}




