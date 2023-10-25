#include "ParagonRenderer.h"

#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "MathHelper.h"
#include "GraphicsResourceHelper.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/RendererChangeList.h"
#include "../ParagonUtil/Log.h"

#include <utility>
#include <singleton-cpp/singleton.h>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;

	ParagonRenderer::ParagonRenderer() :
		_DXStorage(LowDX11Storage::GetInstance()), _DXLogic(LowDX11Logic::GetInstance())
	{
		auto& tRendererChangeList = singleton<Pg::Data::RendererChangeList>();
		_rendererChangeList = &tRendererChangeList;
		
	}

	ParagonRenderer::~ParagonRenderer()
	{

	}

	void ParagonRenderer::BeginRender()
	{
		_DXLogic->PrepareRenderTargets();
		_DXLogic->BindRenderTargets();
		//_DXLogic->SetRasterizerrStates();
	}

	void ParagonRenderer::Render(Pg::Data::CameraData* camData)
	{

		for (auto& it : _renderObject3DList)
		{
			if (it.second->GetBaseRenderer()->GetActive())
			{
				//렌더.
				DirectX::XMFLOAT4X4 tWorldTM = MathHelper::PG2XM_FLOAT4X4(it.first->_transform.GetWorldTM());
				DirectX::XMMATRIX tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);

				DirectX::XMFLOAT4X4 tViewTM = MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
				DirectX::XMMATRIX tViewTMMat = DirectX::XMLoadFloat4x4(&tViewTM);

				DirectX::XMFLOAT4X4 tProjTM = MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
				DirectX::XMMATRIX tProjTMMat = DirectX::XMLoadFloat4x4(&tProjTM);

				it.second->_tempPrimitive->Draw(tWorldTMMat, tViewTMMat, tProjTMMat, DirectX::Colors::Crimson);
			}
		}

		for (auto& it : _renderObject2DList)
		{
			if (it.second->GetBaseRenderer()->GetActive())
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

		//컴포넌트 내부적으로 -> 자신이 어떤 타입인지 Renderer에게 전달. 내부적으로 호출.

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
}




