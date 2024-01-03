#include "GraphicsSceneParser.h"

#include "LayoutDefine.h"
#include "RenderObjectLight.h"
#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"

//세부적인 렌더 오브젝트들의 리스트.
#include "RenderObjectStaticMesh3D.h"
#include "RenderObjectSkinnedMesh3D.h"
#include "RenderObjectText2D.h"
#include "RenderObjectImage2D.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/LightType.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonUtil/Log.h"

//Wireframe RenderObject들.
#include "Axis.h"
#include "Grid.h"

//세부적인 렌더러들의 리스트.
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/SkinnedMeshRenderer.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"

#include <set>
#include <algorithm>

namespace Pg::Graphics
{
	GraphicsSceneParser::GraphicsSceneParser()
	{
		_renderObject2DList = std::make_unique<RenderObject2DList>();
		_renderObject3DList = std::make_unique<RenderObject3DList>();
		_cubeMapList = std::make_unique<RenderObjectCubemapList>();
		_primObjectList = std::make_unique<RenderObjectWireframeList>();
	}

	GraphicsSceneParser::~GraphicsSceneParser()
	{

	}

	void GraphicsSceneParser::Initialize()
	{
		InitializePrimitiveWireframeObjects();
	}

	void GraphicsSceneParser::ParseSceneData(const Pg::Data::Scene* const newScene)
	{
		//Scene을 파싱해서, 실제 렌더되어야 하는 Object를 연동한다.
		//나중에 같은 씬을 유지하는 중에 오브젝트들 중 하나의 렌더러가 꺼진다거나 
		//상황은 아직 유지 못함. 나중에 _rendererChangeList를 활용하면 된다!

		//기존의 직접적 RenderObject 리스트들 클리어.
		_renderObject2DList->_list.clear();
		_renderObject3DList->Clear();

		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//컴포넌트 내부적으로 -> 자신이 어떤 타입인지 Renderer에게 전달. 내부적으로 호출.

		//1. 씬이 실제로 가지고 있는 Material Path들을 전부 다 뽑고, 기본적인 내용을 세팅한다.
		std::set<std::string> tMaterialPathSet;
		for (auto& tGameObject : newScene->GetObjectList())
		{
			// RenderObject
			Pg::Data::BaseRenderer* tBaseRenderer = tGameObject->GetComponent<Pg::Data::BaseRenderer>();

			if (tBaseRenderer != nullptr)
			{
				if (GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 1)
				{
					Pg::Data::RendererBase3D* tBaseR3D = static_cast<Pg::Data::RendererBase3D*>(tBaseRenderer);
					tMaterialPathSet.insert(tBaseR3D->GetMaterialFilePath());
				}
			}
		}

		//2. set의 값을 RenderObject3DList의 Vector로 옮기기. (중복을 없앤 상태에서, 인덱스로 접근 위해)
		for (auto& it : tMaterialPathSet)
		{
			_renderObject3DList->_materialPathSet.push_back(it);
		}

		//2. unordered_map (_renderObject3DList->_list) 세팅.
		for (auto& it : _renderObject3DList->_materialPathSet)
		{
			_renderObject3DList->_list.insert_or_assign(it, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject3D>>>>());
		}
		
		//3. 이제 실제 오브젝트 내부 RenderObject 연동.
		for (auto& tGameObject : newScene->GetObjectList())
		{
			// RenderObject
			Pg::Data::BaseRenderer* tBaseRenderer = tGameObject->GetComponent<Pg::Data::BaseRenderer>();

			if (tBaseRenderer != nullptr)
			{
				//원래는 여기에 Active한지도 검사해야 한다.

				if (GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 1)
				{
					//미리 Material Path를 갖고 있는 RendererBase3D()으로 포인터로 갖고 오기.
					Pg::Data::RendererBase3D* tBaseR3D = static_cast<Pg::Data::RendererBase3D*>(tBaseRenderer);
					std::string tMatPth = tBaseR3D->GetMaterialFilePath();

					//3D
					//StaticMeshRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
					{
						_renderObject3DList->_list.at(tMatPth)->push_back(std::make_pair(tGameObject,
							std::make_unique<RenderObjectStaticMesh3D>(tBaseRenderer)));
					}
					//SkinnedMeshRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
					{
						_renderObject3DList->_list.at(tMatPth)->push_back(std::make_pair(tGameObject,
							std::make_unique<RenderObjectSkinnedMesh3D>(tBaseRenderer)));
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
	
		//실제 리소스를 사용해야 하기에, Initialize에서 현재 호출하고 있지 않음.
		PlaceCubemapList();
	}

	Pg::Graphics::RenderObject2DList* GraphicsSceneParser::GetRenderObject2DList()
	{
		return _renderObject2DList.get();
	}

	Pg::Graphics::RenderObject3DList* GraphicsSceneParser::GetRenderObject3DList()
	{
		return _renderObject3DList.get();
	}

	Pg::Graphics::RenderObjectCubemapList* GraphicsSceneParser::GetRenderObjectCubemapList()
	{
		return _cubeMapList.get();
	}

	Pg::Graphics::RenderObjectWireframeList* GraphicsSceneParser::GetRenderObjectWireframeList()
	{
		return _primObjectList.get();
	}

	void GraphicsSceneParser::InitializePrimitiveWireframeObjects()
	{
		// Primitive RenderObject 투입 + Initialize();
		_primObjectList->_list.push_back(std::make_unique<Grid>());
		_primObjectList->_list.push_back(std::make_unique<Axis>());

		//일괄적으로 BuildBuffers 수행.
		for (auto& it : _primObjectList->_list)
		{
			it->BuildBuffers();
		}
	}

	void GraphicsSceneParser::PlaceCubemapList()
	{
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Data::Enums::eAssetDefine;

		//Index : 1 추가.
		{
			//Cubemap 데이터를 받기.
			auto tCubemapData = GraphicsResourceManager::Instance()->GetResource("../Resources/Textures/room.dds", eAssetDefine::_CUBEMAP);
			_cubeMapList->_list.push_back(static_cast<RenderCubemap*>(tCubemapData.get()));
		}
	}
}