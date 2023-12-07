#include "ParagonRenderer.h"

#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceHelper.h"
#include "MathHelper.h"

#include "DeferredRenderer.h"
#include "Forward3DRenderer.h"
#include "Forward2DRenderer.h"
#include "DebugRenderer.h"

#include "LayoutDefine.h"
#include "../ParagonData/LightType.h"
#include "RenderObjectLight.h"

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

		_debugRenderer = std::make_unique<DebugRenderer>();
		_debugRenderer->Initialize();

		// 내부적으로 DXStorage를 쓰고 있기 때문에 생성자가 아닌 Initialize()에 있어야 함
		_lights = std::make_unique<RenderObjectLightList>();

		//SkinningMk.2
		_tempMultiMesh = new MultimaterialMesh("tFilePath");
	}

	void ParagonRenderer::BeginRender()
	{
		_deferredRenderer->BeginRender();
	}

	void ParagonRenderer::Render(Pg::Data::CameraData* camData)
	{	
		// Deferred 1st Pass
		_deferredRenderer->BindFirstPass();
		_deferredRenderer->RenderFirstPass(_renderObject3DList.get(), camData);
		_deferredRenderer->UnbindFirstPass();

		// Deferred Lighting Pass
		_deferredRenderer->BindLightingPass();
		_deferredRenderer->RenderLight(_lights.get(), camData);
		_deferredRenderer->UnbindLightingPass();

		// Deferred Final Pass
		_deferredRenderer->BindSecondPass();
		_deferredRenderer->RenderSecondPass();
		_deferredRenderer->UnbindSecondPass();

		// Forward
		_forward3dRenderer->Render(*camData);

		//SkinningMk.2
		//_tempMultiMesh->Render(camData);

		_forward2dRenderer->Render(_renderObject2DList.get(), camData);
	}

	void ParagonRenderer::EndRender()
	{
		_DXLogic->Present();
	}

	void ParagonRenderer::ParseSceneData(const Pg::Data::Scene* const newScene)
	{
		//Scene을 파싱해서, 실제 렌더되어야 하는 Object를 연동한다.
		//나중에 같은 씬을 유지하는 중에 오브젝트들 중 하나의 렌더러가 꺼진다거나 
		//상황은 아직 유지 못함. 나중에 _rendererChangeList를 활용하면 된다!
		
		//기존의 직접적 RenderObject 리스트들 클리어.
		_renderObject2DList->_list.clear();
		_renderObject3DList->_list.clear();
		_lights->ClearLightData();

		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//컴포넌트 내부적으로 -> 자신이 어떤 타입인지 Renderer에게 전달. 내부적으로 호출.
		

		//이제 실제 오브젝트 내부 RenderObject 연동.
		for (auto& tGameObject : newScene->GetObjectList())
		{
			// RenderObject
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

			// Light Component가 붙은 오브젝트들을 파싱하여 Light list에 넣는다. 이는 이후에 Lighting Pass에서 사용됨
			Pg::Data::Light* tLightComponent = tGameObject->GetComponent<Pg::Data::Light>();
			if (tLightComponent != nullptr)
			{
				Pg::Data::Transform* tLightTransform = tGameObject->GetComponent<Pg::Data::Transform>();
				_lights->ParseLights(tLightTransform, tLightComponent);
			}

		}

		// 리스트에 파싱된 조명 정보로 라이팅 패스에 쓰일 상수 버퍼를 만든다
		_lights->BuildConstantBuffers();

		// 디퍼드 렌더러의 멤버에 상수 버퍼을 저장해둔다 (패스별 바인딩을 위해)
		_deferredRenderer->_lightingCBs = _lights->_constantBuffers;
		//_deferredRenderer->_firstCBs = _renderObject3DList->_list
		//_deferredRenderer->_secondCBs

		assert(true);
	}

	void ParagonRenderer::DebugRender(Pg::Data::CameraData* camData)
	{
		_debugRenderer->Render(camData);
	}

	void ParagonRenderer::SyncComponentToGraphics(const Pg::Data::Scene* const newScene)
	{
		
	}

	unsigned int ParagonRenderer::Get3DObjectCount()
	{
		return _renderObject3DList->_list.size();
	}

	void ParagonRenderer::PassDebugGeometryData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec)
	{
		_debugRenderer->GetDebugBoxGeometryData(boxColVec);
	}

	//void ParagonRenderer::SyncDebugGeometryToGraphics(const Pg::Data::Scene* const newScene)
	//{
	//
	//}

}