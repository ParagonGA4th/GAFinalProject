#include "ParagonRenderer.h"

#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceHelper.h"
#include "MathHelper.h"

#include "DeferredRenderer.h"
#include "Forward3DRenderer.h"
#include "Forward2DRenderer.h"

#include "LayoutDefine.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/RendererChangeList.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonUtil/Log.h"

//세부적인 렌더러들의 리스트.
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/SkinnedMeshRenderer.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"

//세부적인 렌더 오브젝트들의 리스트.
#include "RenderObjectStaticMesh3D.h"
#include "RenderObjectSkinnedMesh3D.h"
#include "RenderObjectText2D.h"
#include "RenderObjectImage2D.h"

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

		_renderObject2DList = std::make_unique<RenderObject2DList>();
		_renderObject3DList = std::make_unique<RenderObject3DList>();

	}

	ParagonRenderer::~ParagonRenderer()
	{

	}

	void ParagonRenderer::Initialize()
	{		
		_deferredRenderer = std::make_unique<DeferredRenderer>();
		_deferredRenderer->Initialize();

		_forward3dRenderer = std::make_unique<Forward3DRenderer>();
		_forward3dRenderer->Initialize();

		_forward2dRenderer = std::make_unique<Forward2DRenderer>();
		_forward2dRenderer->Initialize();
	}

	void ParagonRenderer::BeginRender()
	{
		//_DXLogic->PrepareRenderTargets();
		//_DXLogic->BindRenderTargets();
		_deferredRenderer->BeginRender();
		
	}

	void ParagonRenderer::Render(Pg::Data::CameraData* camData)
	{
		// 3D 오브젝트 렌더
		// Deferred
		//_deferredRenderer->BindFirstPass();
		//for (auto& it : _renderObject3DList->_list)
		//{
		//	if (it.second->GetBaseRenderer()->GetActive())
		//	{
		//		_deferredRenderer->RenderFirstPass(it.first, *camData);
		//	}
		//}
		//_deferredRenderer->UnbindFirstPass();
		//
		//_deferredRenderer->BindSecondPass();
		//_deferredRenderer->RenderSecondPass();
		//_deferredRenderer->UnbindSecondPass();
		//
		for (auto& it : _renderObject3DList->_list)
		{
			if (it.second->GetBaseRenderer()->GetActive())
			{
				it.second->Render(camData);
			}
		}
		
		// Forward
		_forward3dRenderer->Render(*camData);

		// 2D 오브젝트 렌더 
		_forward2dRenderer->Render(_renderObject2DList.get(), camData);
	}

	void ParagonRenderer::EndRender()
	{
		_DXLogic->Present();
	}

	void ParagonRenderer::SyncComponentToGraphics()
	{
		//using Pg::Data::GameObject;
		//using Pg::Graphics::Helper::GraphicsResourceHelper;
		//
		////새로 들어온 Renderer들 추가!
		//for (auto it : _rendererChangeList->GetAddedRendererList())
		//{
		//	GameObject* tGameObject = it->_object;
		//
		//	if (GraphicsResourceHelper::IsRenderer3D(it->GetRendererTypeName()))
		//	{
		//		//3D
		//		auto tRes = _renderObject3DList->_list.insert_or_assign(tGameObject,
		//			std::make_unique<RenderObject3D>(it));
		//
		//		if (!tRes.second)
		//		{
		//			//Assign
		//			PG_WARN("Already had a renderer (3D), but replaced with a new one.");
		//		}
		//	}
		//	else
		//	{
		//		//2D
		//		auto tRes = _renderObject2DList->_list.insert_or_assign(tGameObject,
		//			std::make_unique<RenderObject2D>(it));
		//
		//		if (!tRes.second)
		//		{
		//			//Assign
		//			PG_WARN("Already had a renderer (2D), but replaced with a new one.");
		//		}
		//	}
		//}
		//
		////새로 들어온 Renderer를 삭제!
		//for (auto it : _rendererChangeList->GetDeletedRendererList())
		//{
		//	GameObject* tGameObject = it->_object;
		//
		//	if (GraphicsResourceHelper::IsRenderer3D(it->GetRendererTypeName()))
		//	{
		//		//3D
		//		_renderObject3DList->_list.erase(tGameObject);
		//	}
		//	else
		//	{
		//		//2D
		//		_renderObject2DList->_list.erase(tGameObject);
		//	}
		//}
	}

	void ParagonRenderer::OnNewSceneStart(Pg::Data::Scene* newScene)
	{
		//Scene을 파싱해서, 실제 렌더되어야 하는 Object를 연동한다.
		//나중에 같은 씬을 유지하는 중에 오브젝트들 중 하나의 렌더러가 꺼진다거나 
		//상황은 아직 유지 못함. 나중에 _rendererChangeList를 활용하면 된다!
		
		//기존의 직접적 RenderObject 리스트들 클리어.
		_renderObject2DList->_list.clear();
		_renderObject3DList->_list.clear();

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
					//StaticMeshRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
					{
						auto tRes = _renderObject3DList->_list.insert_or_assign(tGameObject,
							std::make_unique<RenderObjectStaticMesh3D>(tBaseRenderer));
					}

					//SkinnedMeshRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
					{
						auto tRes = _renderObject3DList->_list.insert_or_assign(tGameObject,
							std::make_unique<RenderObjectSkinnedMesh3D>(tBaseRenderer));
					}
				}
				else if (GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 0)
				{
					//2D
					//TextRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::TextRenderer*).name())) == 0)
					{
						auto tRes = _renderObject2DList->_list.insert_or_assign(tGameObject,
							std::make_unique<RenderObjectText2D>(tBaseRenderer));
					}

					//ImageRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::ImageRenderer*).name())) == 0)
					{
						auto tRes = _renderObject2DList->_list.insert_or_assign(tGameObject,
							std::make_unique<RenderObjectImage2D>(tBaseRenderer));
					}
				}
			}
		}
		assert(true);
	}
}