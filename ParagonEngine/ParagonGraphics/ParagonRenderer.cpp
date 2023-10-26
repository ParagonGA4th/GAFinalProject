#include "ParagonRenderer.h"

#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceHelper.h"
#include "MathHelper.h"

#include "DeferredRenderer.h"
#include "Forward3DRenderer.h"

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
	using Helper::MathHelper;

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

		_forwardRenderer = new Forward3DRenderer();
		_forwardRenderer->Initialize();
	}

	void ParagonRenderer::BeginRender()
	{
		_deferredRenderer->BeginRender();
		
	}

	void ParagonRenderer::Render(Pg::Data::CameraData camData)
	{
		// 3D 오브젝트 렌더
		// Deferred
		for (auto& it : _renderObject3DList)
		{
			if (it.second->_baseRenderer->GetActive())
			{
				_deferredRenderer->RenderFirstPass(it.first, camData);
			}
		}
		_deferredRenderer->RenderSecondPass();

		// Forward
		_forwardRenderer->Render(camData);

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
}