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

		ClearObjectLists();
		ExtractMaterialPaths(newScene);
		SyncRenderObjects(newScene);



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

	void GraphicsSceneParser::ClearObjectLists()
	{
		//기존의 직접적 RenderObject 리스트들 클리어.
		_renderObject2DList->Clear();
		_renderObject3DList->Clear();
	}

	void GraphicsSceneParser::ExtractMaterialPaths(const Pg::Data::Scene* const newScene)
	{
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

		//2. set의 값을 RenderObject3DList의 Vector로 옮기기. (중복을 없앤 상태.)
		for (auto& it : tMaterialPathSet)
		{
			//일단은 Default Material ID를 설정해주기.
			_renderObject3DList->_materialPathSet.push_back(std::make_pair(it, NULL));
		}

		//3. Material Parser에 의해 부여된 MaterialID를 찾아서 순서에 맞게 기록하기. (Material Path Set)
		auto tMatVec = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
		for (auto& it : _renderObject3DList->_materialPathSet)
		{
			//같은 것 찾기.
			auto res = std::find_if(tMatVec.begin(), tMatVec.end(),
				[&it](const std::shared_ptr<Pg::Data::Resources::GraphicsResource>& val)
				-> bool {return (it.first.compare(val->GetFilePath()) == 0); });

			assert(res != tMatVec.end() && "반드시 해당되는 Material을 여기서 찾았어야 한다.");

			RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(res->get());
			unsigned int tMatID = tRenderMat->GetID();
			it.second = tMatID;
		}

		//4. unordered_map (_renderObject3DList->_list) 세팅.
		for (auto& it : _renderObject3DList->_materialPathSet)
		{
			_renderObject3DList->_staticList.insert_or_assign(it.first, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject3D>>>>());
			_renderObject3DList->_skinnedList.insert_or_assign(it.first, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject3D>>>>());
		}

	}

	void GraphicsSceneParser::SyncRenderObjects(const Pg::Data::Scene* const newScene)
	{
		using Pg::Graphics::Helper::GraphicsResourceHelper;

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

					//Material Path Set를 RenderObject3DList에서 찾기(Index), 없으면 로직 에러.
					//auto it = std::find(_renderObject3DList->_materialPathSet.begin(), _renderObject3DList->_materialPathSet.end(), tMatPth);
					
					auto it = std::find_if(_renderObject3DList->_materialPathSet.begin(), _renderObject3DList->_materialPathSet.end(),
						[&tMatPth](const std::pair<std::string, unsigned int>& val)
						-> bool {return (val.first == tMatPth); });
					assert(it != _renderObject3DList->_materialPathSet.end() && "여기에서 걸렸으면 Material Path를 못 찾았음");
					unsigned int tMaterialID = it->second;

					//3D
					//StaticMeshRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
					{
						_renderObject3DList->_staticList.at(tMatPth)->push_back(std::make_pair(tGameObject,
							std::make_unique<RenderObjectStaticMesh3D>(tBaseRenderer, _objectId3dCount, tMaterialID)));
					}
					//SkinnedMeshRenderer
					else if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
					{
						_renderObject3DList->_skinnedList.at(tMatPth)->push_back(std::make_pair(tGameObject,
							std::make_unique<RenderObjectSkinnedMesh3D>(tBaseRenderer, _objectId3dCount, tMaterialID)));
					}

					//ObjectId3d가 겹치지 않도록 ++
					_objectId3dCount++;
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
	}

}