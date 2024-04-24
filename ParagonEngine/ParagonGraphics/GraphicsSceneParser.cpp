#include "GraphicsSceneParser.h"

#include "LayoutDefine.h"
#include "RenderObjectLight.h"
#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "AssetCombinedLoader.h"

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
#include "../ParagonData/ParagonDefines.h"

//SceneInformation.
#include "../ParagonData/Light.h"
#include "../ParagonData/DirectionalLight.h"
#include "../ParagonData/SpotLight.h"
#include "../ParagonData/PointLight.h"

#include <set>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <iterator>

namespace Pg::Graphics
{
	GraphicsSceneParser::GraphicsSceneParser()
	{

	}

	GraphicsSceneParser::~GraphicsSceneParser()
	{

	}

	void GraphicsSceneParser::Initialize()
	{
		//
	}

	void GraphicsSceneParser::ParseSceneData(const Pg::Data::Scene* const newScene)
	{
		//Scene을 파싱해서, 실제 렌더되어야 하는 Object를 연동한다.
		//나중에 같은 씬을 유지하는 중에 오브젝트들 중 하나의 렌더러가 꺼진다거나 
		//상황은 아직 유지 못함. 나중에 _rendererChangeList를 활용하면 된다!

		ResetParser();

		PlacePathsFromName(newScene);
		CheckForPathNameErrors(newScene);

		ExtractMaterialPaths(newScene);
		SyncRenderObjects(newScene);
		RemapMaterialIdAll();
		SetupPrimitiveWireframeObjects();
		SyncSceneAllLights(newScene);
		CheckBindAdequateFunctions();
		//이제 별도로 렌더링과 관련된 오브젝트들을 받아야 한다.

		CheckCreateObjMatBuffersAll();

		//실제 리소스를 사용해야 하기에, Initialize에서 현재 호출하고 있지 않음.
		PlaceCubemapList();


		assert("");
	}

	void GraphicsSceneParser::HandleRenderObjectsRuntime()
	{
		///미리 로드되었던 오브젝트를 없애려고 할 경우, Delete가 호출되지 않는다.
		///Modify도 갑자기 Add Logic을 발생시킨다.
		///버그 발생 경우 : test_colorCorrect에서 오브젝트 자체를 삭제하려고 할 때.
		///처음부터 생성된 오브젝트들이 감지 안되는 것인가?

		//CheckCreate는 쓸데없는 파싱 비용이 든다.
		//미리 체크해서, EarlyReturn 가능하면 하기!
		if (_runtimeAddedObjectList.empty() &&
			_runtimeModifiedObjectList.empty() &&
			_runtimeDeletedObjectList.empty())
		{
			return;
		}

		//Added Objects : 미리 PlaceCorrectPath & Error Check.
		std::for_each(_runtimeAddedObjectList.begin(), _runtimeAddedObjectList.end(), [this](Pg::Data::GameObject*& it)
			{
				PlaceCorrectPathSingleRenderer(it);
				CheckPathNameErrorSingleRenderer(it);
			});

		//Modified Objects : 미리 PlaceCorrectPath & Error Check.
		std::for_each(_runtimeModifiedObjectList.begin(), _runtimeModifiedObjectList.end(), [this](Pg::Data::GameObject*& it)
			{
				PlaceCorrectPathSingleRenderer(it);
				CheckPathNameErrorSingleRenderer(it);
			});

		//미리 Material을 추가 / 삭제해야 한다면, 미리 값을 받아야 한다.
		RuntimeExtractMaterialPathsVector(&_runtimeAddedObjectList);
		RuntimeExtractMaterialPathsVector(&_runtimeModifiedObjectList);

		//Skinned일 경우, BindAdequateFunctions 역시 호출되어야 한다!
		//Add 처리.
		//별도로 ObjectBuffer 만들어야 함.-> Toggle해야 (기본적으로 되어 있음)
		
		for (auto& it : _runtimeAddedObjectList)
		{
			AddSingleRenderObject(it);
		}
		for (auto& it : _runtimeModifiedObjectList)
		{
			ModifySingleRenderObject(it);
		}
		for (auto& it : _runtimeDeletedObjectList)
		{
			DeleteSingleRenderObject(it);
		}

		//추가된 것만 검사해서 만든다!
		RemapAppendedMatID();

		//Light 관련된 일관된 연관.
		RuntimeAddLightsVector(&_runtimeAddedObjectList);
		RuntimeModifyLightsVector(&_runtimeModifiedObjectList);
		RuntimeDeleteLightsVector(&_runtimeDeletedObjectList);


		//EarlyReturn이 안되었다는 건 뭐라도 바꿀게 있다는 말.
		//SceneParser 내부에 추가된 / 변경된 오브젝트만 가지고 호출하자.
		//미리 받아서 하면 오류 생길 것 같다.
		CheckBindAdequateFunctions();
		CheckCreateObjMatBuffersAll();

		///TODO : 그리고 중간에 Material이 새롭게 발견되면 미리 해당 옵젝들만 관련해서 MaterialPath를 새로 만들어 넣어줘야 한다.
		///

		//일괄적으로 Clear & Update.
		if (!_runtimeAddedObjectList.empty())
		{
			_runtimeAddedObjectList.clear();
		}

		if (!_runtimeModifiedObjectList.empty())
		{
			_runtimeModifiedObjectList.clear();
		}

		if (!_runtimeDeletedObjectList.empty())
		{
			_runtimeDeletedObjectList.clear();
		}
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

	Pg::Graphics::SceneInformationList* GraphicsSceneParser::GetSceneInformationList()
	{
		return _sceneInfoList.get();
	}

	void GraphicsSceneParser::SetupPrimitiveWireframeObjects()
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
			auto tCubemapData = GraphicsResourceManager::Instance()->GetResource(Pg::Defines::ASSET_DEFAULT_ENVIRONMENT_CUBEMAP_PATH, eAssetDefine::_CUBEMAP);
			_cubeMapList->_list.push_back(static_cast<RenderCubemap*>(tCubemapData.get()));
		}
	}

	void GraphicsSceneParser::ResetParser()
	{
		//기존의 직접적 RenderObject 리스트들 클리어. + SceneInformationList.
		_renderObject2DList.reset(new RenderObject2DList());
		_renderObject3DList.reset(new RenderObject3DList());
		_cubeMapList.reset(new RenderObjectCubemapList());
		_primObjectList.reset(new RenderObjectWireframeList());
		_sceneInfoList.reset(new SceneInformationList());

		//새로운 씬을 로드하니, 자체적인 Object Count를 리셋한다.
		//씬이 바뀌기 전까지는 ObjectId3dCount가 바뀌지 않을 것.
		this->_objectId3dCount = 1;

		//Material에 InitState를 전부 다 None으로 부여.
		Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetCombinedLoader()->ResetAllKnownMatInitStates();

	}

	void GraphicsSceneParser::PlacePathsFromName(const Pg::Data::Scene* const newScene)
	{
		PG_WARN("Mesh File Path가 비어있을 때만 Unreal 컨버터가 작동할 것. 유의.");
		//std::for_each(newScene->GetObjectList().begin(), newScene->GetObjectList().end(), std::mem_fn(&PlaceCorrectPathSingleRenderer));
		for (auto& it : newScene->GetObjectList())
		{
			PlaceCorrectPathSingleRenderer(it);
		}
	}

	void GraphicsSceneParser::CheckForPathNameErrors(const Pg::Data::Scene* const newScene)
	{
		for (auto& it : newScene->GetObjectList())
		{
			CheckPathNameErrorSingleRenderer(it);
		}
		//std::for_each(newScene->GetObjectList().begin(), newScene->GetObjectList().end(), std::mem_fn(&CheckPathNameErrorSingleRenderer));
	}

	void GraphicsSceneParser::ExtractMaterialPaths(const Pg::Data::Scene* const newScene)
	{
		//컴포넌트 내부적으로 -> 자신이 어떤 타입인지 Renderer에게 전달. 내부적으로 호출.
		UINT tAddedDefaultMatCount;
		RuntimeExtractMaterialPathsVector(&(newScene->GetObjectList()));
	}

	void GraphicsSceneParser::SyncRenderObjects(const Pg::Data::Scene* const newScene)
	{
		//3. 이제 실제 오브젝트 내부 RenderObject 연동.
		//std::for_each(newScene->GetObjectList().begin(), newScene->GetObjectList().end(), std::mem_fn(&AddSingleRenderObject));

		for (auto& it : newScene->GetObjectList())
		{
			AddSingleRenderObject(it);
		}
	}

	void GraphicsSceneParser::AddSingleRenderObject(Pg::Data::GameObject* obj)
	{
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		auto tComponentVector = obj->GetComponents<Pg::Data::BaseRenderer>();
		if (tComponentVector.empty())
		{
			return;
		}
		for (int i = 0; i < tComponentVector.size(); i++)
		{
			// RenderObject
			Pg::Data::BaseRenderer* tBaseRenderer = tComponentVector.at(i);
			assert(tBaseRenderer != nullptr && "이 시점에서는 반드시 있어야 한다.");

			//원래는 여기에 Active한지도 검사해야 한다.
			if (GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 1)
			{
				//미리 Material Path를 갖고 있는 RendererBase3D()으로 포인터로 갖고 오기.
				Pg::Data::RendererBase3D* tBaseR3D = static_cast<Pg::Data::RendererBase3D*>(tBaseRenderer);
				std::string tMatPth = tBaseR3D->GetMaterialFilePath();

				//Material Path Set를 RenderObject3DList에서 찾기(Index), 없으면 로직 에러.
				//auto it = std::find(_renderObject3DList->_materialPathSet.begin(), _renderObject3DList->_materialPathSet.end(), tMatPth);

				auto it = std::find_if(_renderObject3DList->_materialPathSet.begin(), _renderObject3DList->_materialPathSet.end(),
					[&tMatPth](const std::pair<std::string, RenderMaterial*>& val)
					-> bool {return (val.first == tMatPth); });
				//곧 들어갈 Material ID;

				//unsigned int tMaterialID = NULL;
				RenderMaterial* tMaterialInput = nullptr;

				//못 찾았으면, Default Material을 만들어서 넣어준다.
				if (it == _renderObject3DList->_materialPathSet.end())
				{
					//CreateDefaultMaterialInstance;
					std::filesystem::path tTempMeshPath = tBaseR3D->GetMeshFilePath();
					std::string tTempMeshName = tTempMeshPath.filename().string();
					std::string tDefaultMatInstName = Pg::Graphics::Helper::GraphicsResourceHelper::GetDefaultMaterialNameFromMeshName(tTempMeshName);

					if (!(Pg::Graphics::Manager::GraphicsResourceManager::Instance()->IsExistDefaultMaterialByMeshName(tTempMeshName)))
					{
						//MeshName으로 만들어진 Default Material이 아직 없다.

						//DefaultMaterial 로드할 것이다. "DefaultMaterial_"이 들어있기 때문에, 디폴트로 로드될 것이다.
						Pg::Graphics::Manager::GraphicsResourceManager::Instance()->LoadResource(tDefaultMatInstName, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);

						//전체 저장목록에 갖고 있다고 기록. (Graphics에서 검사했기 때문에, AssetManager로 보내줘야)
						Pg::Graphics::Manager::GraphicsResourceManager::Instance()->AddSecondaryResource(tDefaultMatInstName, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);
					}

					//씬 재시작 경우의 수를 위해, 없을 경우에는 로드만 하고 일괄적으로 insert/assign하는 것으로 변경.
					//이제는, vector 목록에 추가해줘야.
					// => 어떻게 되었든, Skinned/Static에 없는 것이니 넣어!
					auto res = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(tDefaultMatInstName, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);
					RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(res.get());

					//런타임에서 오브젝트를 파싱해주며 만들어주는 특성상, => 무조건 있는지 체크해야. 
					//이미 존재할 시에는 넣어주면 안됨.
					//있으면 새로운 벡터를 만들지 않음. (insert_or_assign에서 TryEmplace로 변경)
					
					_renderObject3DList->_staticList.try_emplace(tRenderMat, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectStaticMesh3D>>>>());
					_renderObject3DList->_skinnedList.try_emplace(tRenderMat, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectSkinnedMesh3D>>>>());

					//일단은 Default Material ID를 설정해주기.
					_renderObject3DList->_materialPathSet.try_emplace(tDefaultMatInstName, tRenderMat);

					//일괄적 부여를 위해 Material 포인터 부여.
					tMaterialInput = tRenderMat;

					//로직 후 디버깅.
					std::string tMsg = obj->GetName();
					tMsg += " : 디폴트 매터리얼 객체 사용됨.";

					//나머지 로직과 호환되게 변수 재할당. 
					tMatPth = tDefaultMatInstName;

					//PG_TRACE(tMsg.c_str());
				}
				else
				{
					tMaterialInput = it->second;
				}

				//3D
				//알파블렌딩을 사용하는 경우
				if (tMaterialInput->GetIsUseAlphaBlending())
				{
					//StaticMeshRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
					{
						auto tUPtr = std::make_unique<AlphaBlendedTuple>(obj, tMaterialInput, false);
						tUPtr->_eitherStaticMesh = std::make_unique<RenderObjectStaticMesh3D>(tBaseRenderer, _objectId3dCount);
						tUPtr->_eitherStaticMesh->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));

						_renderObject3DList->_allAlphaBlendedList.push_back(std::move(tUPtr));
					}
					//SkinnedMeshRenderer
					else if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
					{
						auto tUPtr = std::make_unique<AlphaBlendedTuple>(obj, tMaterialInput, true);
						tUPtr->_eitherSkinnedMesh = std::make_unique<RenderObjectSkinnedMesh3D>(tBaseRenderer, _objectId3dCount);
						tUPtr->_eitherSkinnedMesh->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));

						_renderObject3DList->_allAlphaBlendedList.push_back(std::move(tUPtr));
					}
				}
				else //그냥 불투명 렌더링.
				{
					//StaticMeshRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
					{
						_renderObject3DList->_staticList.at(tMaterialInput)->push_back(std::make_pair(obj,
							std::make_unique<RenderObjectStaticMesh3D>(tBaseRenderer, _objectId3dCount)));

						//개별적으로 MaterialID 할당. 값이랑은 무관할 것이다.
						_renderObject3DList->_staticList.at(tMaterialInput)->back().second->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));
					}
					//SkinnedMeshRenderer
					else if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
					{
						_renderObject3DList->_skinnedList.at(tMaterialInput)->push_back(std::make_pair(obj,
							std::make_unique<RenderObjectSkinnedMesh3D>(tBaseRenderer, _objectId3dCount)));

						//개별적으로 MaterialID 할당. 값이랑은 무관할 것이다.
						_renderObject3DList->_skinnedList.at(tMaterialInput)->back().second->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));
					}
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
					_renderObject2DList->_list.push_back(std::make_pair(obj,
						std::make_unique<RenderObjectText2D>(tBaseRenderer)));
				}

				//ImageRenderer
				if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::ImageRenderer*).name())) == 0)
				{
					_renderObject2DList->_list.push_back(std::make_pair(obj,
						std::make_unique<RenderObjectImage2D>(tBaseRenderer)));
				}
			}
		}
	}

	void GraphicsSceneParser::SyncSceneAllLights(const Pg::Data::Scene* const newScene)
	{
		RuntimeAddLightsVector(&(newScene->GetObjectList()));
	}

	void GraphicsSceneParser::CheckBindAdequateFunctions()
	{
		using Pg::Graphics::Helper::GraphicsResourceHelper;


		for (auto& it : _renderObject3DList->_skinnedList)
		{
			for (auto& [go, ro] : *(it.second.get()))
			{
				RenderObjectSkinnedMesh3D* tSkinnedRO = static_cast<RenderObjectSkinnedMesh3D*>(ro.get());
				Pg::Data::SkinnedMeshRenderer* tSkinnedRenderer = static_cast<Pg::Data::SkinnedMeshRenderer*>(tSkinnedRO->GetBaseRenderer());
				//std::bind로 Data쪽에서 원격으로 함수를 호출할 수 있게.
				//준비 안되었을 때만,
				if (!(ro->_isInternalUpToDate))
				{
					tSkinnedRenderer->_setAnimationFunction = std::bind(&RenderObjectSkinnedMesh3D::SetAnimation, tSkinnedRO, std::placeholders::_1, std::placeholders::_2);
					tSkinnedRenderer->_findAnimTransformFunction = std::bind(&RenderObjectSkinnedMesh3D::FindAnimTransform, tSkinnedRO, std::placeholders::_1);
				}
			}
		}
	}

	void GraphicsSceneParser::CheckCreateObjMatBuffersAll()
	{
		//모든 오브젝트 렌더링. (static)
		for (auto& it : _renderObject3DList->_staticList)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				//ObjectMaterial Buffer가 만들어지지 않은 애들만 호출, 실행.
				if (!(it.second->at(i).second->_isInternalUpToDate))
				{
					it.second->at(i).second->CreateObjMatBuffers();
					//세팅 됨.
					it.second->at(i).second->_isInternalUpToDate = true;
				}
			}
		}

		//모든 오브젝트 렌더링. (skinned)
		for (auto& it : _renderObject3DList->_skinnedList)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				//ObjectMaterial Buffer가 만들어지지 않은 애들만 호출, 실행.
				if (!(it.second->at(i).second->_isInternalUpToDate))
				{
					it.second->at(i).second->CreateObjMatBuffers();
					//세팅 됨.
					it.second->at(i).second->_isInternalUpToDate = true;
				}
			}
		}
	}

	Pg::Data::GameObject* GraphicsSceneParser::GetObjectWithObjID(unsigned int objID)
	{
		Pg::Data::GameObject* tRet = nullptr;

		//Static List 내부 찾기.
		for (auto& [bMatPath, bVectorPtr] : _renderObject3DList->_staticList)
		{
			for (int i = 0; i < bVectorPtr->size(); i++)
			{
				auto& [go, ro] = bVectorPtr->at(i);

				if (ro->GetObjectID() == objID)
				{
					tRet = go;
					goto gtFinished;
				}
			}
		}

		//Skinned List 내부 찾기.
		for (auto& [bMatPath, bVectorPtr] : _renderObject3DList->_skinnedList)
		{
			for (int i = 0; i < bVectorPtr->size(); i++)
			{
				auto& [go, ro] = bVectorPtr->at(i);

				if (ro->GetObjectID() == objID)
				{
					tRet = go;
					goto gtFinished;
				}
			}
		}

		//유일하게 Goto 사용이 허용되는 예시 : nested loops, in single functions.
	gtFinished:
		assert(tRet != nullptr && "무조건 Picking한 GameObject를 찾았어야 하는 함수에서 값을 찾지 못했다.");
		//PG_TRACE(tRet->GetName().c_str());
		return tRet;
	}

	unsigned int GraphicsSceneParser::GetObjIDWithObject(const Pg::Data::GameObject* const obj)
	{
		unsigned int tRet = NULL;

		//Static List 내부 찾기.
		for (auto& [bMatPath, bVectorPtr] : _renderObject3DList->_staticList)
		{
			for (int i = 0; i < bVectorPtr->size(); i++)
			{
				auto& [go, ro] = bVectorPtr->at(i);

				if (go == obj)
				{
					tRet = ro->GetObjectID();
					goto jobFinished;
				}
			}
		}

		//Skinned List 내부 찾기.
		for (auto& [bMatPath, bVectorPtr] : _renderObject3DList->_skinnedList)
		{
			for (int i = 0; i < bVectorPtr->size(); i++)
			{
				auto& [go, ro] = bVectorPtr->at(i);

				if (go == obj)
				{
					tRet = ro->GetObjectID();
					goto jobFinished;
				}
			}
		}

		//유일하게 Goto 사용이 허용되는 예시 : nested loops, in single functions.
	jobFinished:
		//만약 tRet이 NULL이라면, Render되지 않는 게임오브젝트를 선택했다는 뜻.
		//한번 필터링되어오기 때문에, 해당 상황은 불가.

		///이때, 일단 저거 끄기로 했음! 불가능한 상황은 외부적으로 무시하게 설정했기 때문.
		//assert(tRet != NULL && "불가능한 상황, 필터링되어 온 게임오브젝트의 렌더 목록에서 발견X");

		return tRet;
	}

	void GraphicsSceneParser::AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{
		std::copy(objVecP->begin(), objVecP->end(), std::back_inserter(_runtimeAddedObjectList));
	}

	void GraphicsSceneParser::ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{
		std::copy(objVecP->begin(), objVecP->end(), std::back_inserter(_runtimeModifiedObjectList));
	}

	void GraphicsSceneParser::DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{
		std::copy(objVecP->begin(), objVecP->end(), std::back_inserter(_runtimeDeletedObjectList));
	}

	void GraphicsSceneParser::ModifySingleRenderObject(Pg::Data::GameObject* obj)
	{	
		//뭐가 되었든, 다시 지우고 바로 만들거나 / 그렇지 않아야 한다.
		
		//일단, 지금 오브젝트가 렌더러를 가지고 있는지부터.
		auto renderVec = obj->GetComponents<Pg::Data::BaseRenderer>();
		if (renderVec.empty())
		{
			//해당 컴포넌트 있던 겜옵젝 -> 컴포넌트가 사라지는 경우. (없으면 아무것도 하지 않음)
			_renderObject2DList->DeleteRenderObjectWithGameObject(obj);
			_renderObject3DList->DeleteRenderObjectWithGameObject(obj);

			return;
		}

		auto tRo2d = _renderObject2DList->GetRenderObjectWithGameObject(obj);
		auto tRo3d = _renderObject3DList->GetRenderObjectWithGameObject(obj);
		if (tRo2d.empty() && tRo3d.empty())
		{
			// 1. 해당 컴포넌트 없는 겜옵젝 -> 컴포넌트가 생기는 경우.
			//원래 없는데, 지금은 생긴 것!
			AddSingleRenderObject(obj);

			return;
		}

		// 2. 해당 컴포넌트 있던 겜옵젝 -> 컴포넌트 값이 수정되는 경우.
		//어쨌든 지우고 다시 만들어야 한다.
		_renderObject2DList->DeleteRenderObjectWithGameObject(obj);
		_renderObject3DList->DeleteRenderObjectWithGameObject(obj);
		AddSingleRenderObject(obj);

	}

	void GraphicsSceneParser::DeleteSingleRenderObject(Pg::Data::GameObject* obj)
	{
		auto renderVec = obj->GetComponents<Pg::Data::BaseRenderer>();

		//(없으면 아무것도 하지 않음)
		_renderObject2DList->DeleteRenderObjectWithGameObject(obj);
		_renderObject3DList->DeleteRenderObjectWithGameObject(obj);
	}
	void GraphicsSceneParser::PlaceCorrectPathSingleRenderer(Pg::Data::GameObject* tGameObject)
	{
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Data::Enums::eAssetDefine;
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		// RenderObject
		Pg::Data::BaseRenderer* tBaseRenderer = tGameObject->GetComponent<Pg::Data::BaseRenderer>();

		if (tBaseRenderer != nullptr && GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 1)
		{
			Pg::Data::RendererBase3D* tBaseR3D = static_cast<Pg::Data::RendererBase3D*>(tBaseRenderer);

			if (!tBaseR3D->GetMeshFilePath().empty())
			{
				//이미 MeshFilePath가 있다는 뜻이다.
				//현재 시스템 상으로 들어간 값. 
				//호환을 위해 해당 코드 작성.
				return;
			}

			//언리얼식 표기 바꾸기. (만약 해당될 경우)
			tBaseR3D->ConvertPotentialUnrealValues();

			//저장된 Name 가지고 GraphicsResourceManager에서 상호 확인 :
			//전체 Path 명시 + 정리.
			if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
			{
				//렌더러 정보.
				Pg::Data::StaticMeshRenderer* tActualRenderer = static_cast<Pg::Data::StaticMeshRenderer*>(tBaseRenderer);

				//리소스 매니저에서 확인 -> MeshName / MeshPath.
				//무조건 이 시점에서는 존재해야 한다.
				tActualRenderer->SetMeshFilePath(Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResourcePathByName(
					tActualRenderer->_meshName, eAssetDefine::_3DMODEL));

				//리소스 매니저에서 확인 -> MaterialName / MaterialPath
				if (!tActualRenderer->_materialName.empty())
				{
					tActualRenderer->SetMaterialFilePath(Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResourcePathByName(
						tActualRenderer->_materialName, eAssetDefine::_RENDERMATERIAL));
				}

				//Mesh Path Set / 만약 Default Material이 아닌 경우 MaterialPath까지 배치 완료.
			}
			else if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
			{
				//Skinned를 연동하려고 하니, 모두 추가되어야 한다.
				//렌더러 정보.
				Pg::Data::SkinnedMeshRenderer* tActualRenderer = static_cast<Pg::Data::SkinnedMeshRenderer*>(tBaseRenderer);

				//리소스 매니저에서 확인 -> MeshName / MeshPath.
				//무조건 이 시점에서는 존재해야 한다.
				tActualRenderer->SetMeshFilePath(Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResourcePathByName(
					tActualRenderer->_meshName, eAssetDefine::_3DMODEL));

				//리소스 매니저에서 확인 -> MaterialName / MaterialPath
				if (!tActualRenderer->_materialName.empty())
				{
					tActualRenderer->SetMaterialFilePath(Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResourcePathByName(
						tActualRenderer->_materialName, eAssetDefine::_RENDERMATERIAL));
				}

				//Mesh Path Set / 만약 Default Material이 아닌 경우 MaterialPath까지 배치 완료.
			}

			//모든 Conversion이 끝난 후일 것이다. 
		}
	}

	void GraphicsSceneParser::CheckPathNameErrorSingleRenderer(Pg::Data::GameObject* tGameObject)
	{
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Data::Enums::eAssetDefine;
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		// RenderObject
		Pg::Data::BaseRenderer* tBaseRenderer = tGameObject->GetComponent<Pg::Data::BaseRenderer>();

		if (tBaseRenderer != nullptr && GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 1)
		{
			Pg::Data::RendererBase3D* tBaseR3D = static_cast<Pg::Data::RendererBase3D*>(tBaseRenderer);
			//이름<->경로 연동 더블 체크.
			tBaseR3D->CheckForPathNameErrors();
		}
	}

	void GraphicsSceneParser::RuntimeExtractMaterialPathsVector(const std::vector<Pg::Data::GameObject*>* runtimeObjList)
	{
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//1. 씬이 실제로 가지고 있는 Material Path들을 전부 다 뽑고, 기본적인 내용을 세팅한다.
		std::set<std::string> tMaterialPathSet;
		for (auto& tGameObject : *runtimeObjList)
		{
			// RenderObject
			Pg::Data::BaseRenderer* tBaseRenderer = tGameObject->GetComponent<Pg::Data::BaseRenderer>();

			if (tBaseRenderer != nullptr)
			{
				if (GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 1)
				{
					Pg::Data::RendererBase3D* tBaseR3D = static_cast<Pg::Data::RendererBase3D*>(tBaseRenderer);

					//만약 비어있지 않다면 (비어있으면 Default Material)
					if (!tBaseR3D->GetMaterialFilePath().empty())
					{
						tMaterialPathSet.insert(tBaseR3D->GetMaterialFilePath());
					}
				}
			}
		}

		//2. set의 값을 RenderObject3DList의 Vector로 옮기기. (중복을 없앤 상태.)
		//저장되는 정보 : 이미 있으면 넣으면 안됨.
		for (auto& it : tMaterialPathSet)
		{
			auto res = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(it, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);
			RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(res.get());
			//일단은 Default Material ID를 설정해주기.
			_renderObject3DList->_materialPathSet.try_emplace(it, tRenderMat);
		}

		//ID는 이제 상관 없다. 따로 일괄적으로 부여하기 때문에.
		//기록하는 방식을 String -> RenderMat 포인터로 변경!

		//3. unordered_map (_renderObject3DList->_list) 세팅.
		for (auto& it : _renderObject3DList->_materialPathSet)
		{
			//중요 : 이미 존재하면 새로 만들면 안된다.
			_renderObject3DList->_staticList.try_emplace(it.second, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectStaticMesh3D>>>>());
			_renderObject3DList->_skinnedList.try_emplace(it.second, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectSkinnedMesh3D>>>>());
		}
	}

	void GraphicsSceneParser::RuntimeAddLightsVector(const std::vector<Pg::Data::GameObject*>* objList)
	{
		//비어있으면 Early Return.
		if (objList->empty())
		{
			return;
		}

		//Light들의 Component 리스트 일단은 연동.
		for (auto& tGameObject : *objList)
		{
			//1. 라이트가 있는지 체크한다.
			auto tLightComponentVector = tGameObject->GetComponents<Pg::Data::Light>();

			for (auto& tSingleLight : tLightComponentVector)
			{
				//Directional Light일 경우.
				Pg::Data::DirectionalLight* tDirLight = dynamic_cast<Pg::Data::DirectionalLight*>(tSingleLight);
				if (tDirLight != nullptr)
				{
					_sceneInfoList->_dirLightList.push_back(tDirLight);
					continue;
				}

				//Spot Light일 경우.
				Pg::Data::SpotLight* tSpotLight = dynamic_cast<Pg::Data::SpotLight*>(tSingleLight);
				if (tSpotLight != nullptr)
				{
					_sceneInfoList->_spotLightList.push_back(tSpotLight);
					continue;
				}

				//Point Light일 경우.
				Pg::Data::PointLight* tPointLight = dynamic_cast<Pg::Data::PointLight*>(tSingleLight);
				if (tPointLight != nullptr)
				{
					_sceneInfoList->_pointLightList.push_back(tPointLight);
					continue;
				}
			}
		}
		//다 넣었으니, 전체 Sorting 다시.
		SortSceneInfoLightsVector();
	}

	void GraphicsSceneParser::RuntimeModifyLightsVector(const std::vector<Pg::Data::GameObject*>* objList)
	{
		//비어있으면 Early Return.
		if (objList->empty())
		{
			return;
		}

		///총 3가지 경우.
		/// 1. 해당 컴포넌트 없는 겜옵젝 -> 컴포넌트가 생기는 경우.
		/// 2. 해당 컴포넌트 있던 겜옵젝 -> 컴포넌트가 사라지는 경우.
		/// 3. 해당 컴포넌트 있던 겜옵젝 -> 컴포넌트가 수정되는 경우.
	
		std::vector<Pg::Data::DirectionalLight*> tEnteredDirLights;
		std::vector<Pg::Data::SpotLight*> tEnteredSpotLights;
		std::vector<Pg::Data::PointLight*> tEnteredPointLights;

		//Light들의 Component 리스트 별도로 저장.
		for (auto& tGameObject : *objList)
		{
			//1. 라이트가 있는지 체크한다.
			auto tLightComponentVector = tGameObject->GetComponents<Pg::Data::Light>();

			for (auto& tSingleLight : tLightComponentVector)
			{
				//Directional Light일 경우.
				Pg::Data::DirectionalLight* tDirLight = dynamic_cast<Pg::Data::DirectionalLight*>(tSingleLight);
				if (tDirLight != nullptr)
				{
					tEnteredDirLights.push_back(tDirLight);
					continue;
				}

				//Spot Light일 경우.
				Pg::Data::SpotLight* tSpotLight = dynamic_cast<Pg::Data::SpotLight*>(tSingleLight);
				if (tSpotLight != nullptr)
				{
					tEnteredSpotLights.push_back(tSpotLight);
					continue;
				}

				//Point Light일 경우.
				Pg::Data::PointLight* tPointLight = dynamic_cast<Pg::Data::PointLight*>(tSingleLight);
				if (tPointLight != nullptr)
				{
					tEnteredPointLights.push_back(tPointLight);
					continue;
				}
			}
		}

		//이제 SceneInfo 리스트에서 범위 안에 있으면 다 삭제하기.
		//이미 있는지 체크. 
		//Filter해서 겹치는 것은 삭제.
		{
			auto tIsDirAlready = [&](Pg::Data::DirectionalLight* val)
				{
					//임시 리스트에서 찾았다는 얘기 -> 그러니까, 밑의 Erase-Remove_If Idiom에서 찾은 애들만 지우라는 말!
					return std::ranges::find(tEnteredDirLights, val) != tEnteredDirLights.end();
				};
			_sceneInfoList->_dirLightList.erase(std::remove_if(_sceneInfoList->_dirLightList.begin(),
				_sceneInfoList->_dirLightList.end(), tIsDirAlready), _sceneInfoList->_dirLightList.end());
		}
		{
			auto tIsSpotAlready = [&](Pg::Data::SpotLight* val)
				{
					//임시 리스트에서 찾았다는 얘기 -> 그러니까, 밑의 Erase-Remove_If Idiom에서 찾은 애들만 지우라는 말!
					return std::ranges::find(tEnteredSpotLights, val) != tEnteredSpotLights.end();
				};
			_sceneInfoList->_spotLightList.erase(std::remove_if(_sceneInfoList->_spotLightList.begin(),
				_sceneInfoList->_spotLightList.end(), tIsSpotAlready), _sceneInfoList->_spotLightList.end());
		}
		{
			auto tIsPointAlready = [&](Pg::Data::PointLight* val)
				{
					//임시 리스트에서 찾았다는 얘기 -> 그러니까, 밑의 Erase-Remove_If Idiom에서 찾은 애들만 지우라는 말!
					return std::ranges::find(tEnteredPointLights, val) != tEnteredPointLights.end();
				};
			_sceneInfoList->_pointLightList.erase(std::remove_if(_sceneInfoList->_pointLightList.begin(),
				_sceneInfoList->_pointLightList.end(), tIsPointAlready), _sceneInfoList->_pointLightList.end());
		}

		//겹친거 삭제한 다음에, 다시 값을 넣는다. (수정이기 때문)
		std::copy(tEnteredDirLights.begin(), tEnteredDirLights.end(), std::back_inserter(_sceneInfoList->_dirLightList));
		std::copy(tEnteredSpotLights.begin(), tEnteredSpotLights.end(), std::back_inserter(_sceneInfoList->_spotLightList));
		std::copy(tEnteredPointLights.begin(), tEnteredPointLights.end(), std::back_inserter(_sceneInfoList->_pointLightList));
		
		//Data 자체를 가져다가 쓰기 때문에, 같은 경우는 별도의 연동이 필요 없음.
		//Intensity를 기반으로 Sort. ( '>' Operator Overloading )
		//그냥 데이터 Sort만 다시 돌리면 된다!
		SortSceneInfoLightsVector();
	}

	void GraphicsSceneParser::RuntimeDeleteLightsVector(const std::vector<Pg::Data::GameObject*>* objList)
	{
		//비어있으면 Early Return.
		if (objList->empty())
		{
			return;
		}

		std::vector<Pg::Data::DirectionalLight*> tEnteredDirLights;
		std::vector<Pg::Data::SpotLight*> tEnteredSpotLights;
		std::vector<Pg::Data::PointLight*> tEnteredPointLights;
		
		//Light들의 Component 리스트 별도로 저장.
		for (auto& tGameObject : *objList)
		{
			//1. 라이트가 있는지 체크한다.
			auto tLightComponentVector = tGameObject->GetComponents<Pg::Data::Light>();
		
			for (auto& tSingleLight : tLightComponentVector)
			{
				//Directional Light일 경우.
				Pg::Data::DirectionalLight* tDirLight = dynamic_cast<Pg::Data::DirectionalLight*>(tSingleLight);
				if (tDirLight != nullptr)
				{
					tEnteredDirLights.push_back(tDirLight);
					continue;
				}
		
				//Spot Light일 경우.
				Pg::Data::SpotLight* tSpotLight = dynamic_cast<Pg::Data::SpotLight*>(tSingleLight);
				if (tSpotLight != nullptr)
				{
					tEnteredSpotLights.push_back(tSpotLight);
					continue;
				}
		
				//Point Light일 경우.
				Pg::Data::PointLight* tPointLight = dynamic_cast<Pg::Data::PointLight*>(tSingleLight);
				if (tPointLight != nullptr)
				{
					tEnteredPointLights.push_back(tPointLight);
					continue;
				}
			}
		}

		//이제 SceneInfo 리스트에서 범위 안에 있으면 다 삭제하기.
		//이미 있는지 체크. 
		//Filter해서 겹치는 것은 삭제.
		{
			auto tIsDirAlready = [&](Pg::Data::DirectionalLight* val)
				{
					//임시 리스트에서 찾았다는 얘기 -> 그러니까, 밑의 Erase-Remove_If Idiom에서 찾은 애들만 지우라는 말!
					return std::ranges::find(tEnteredDirLights, val) != tEnteredDirLights.end();
				};
			_sceneInfoList->_dirLightList.erase(std::remove_if(_sceneInfoList->_dirLightList.begin(),
				_sceneInfoList->_dirLightList.end(), tIsDirAlready), _sceneInfoList->_dirLightList.end());
		}
		{
			auto tIsSpotAlready = [&](Pg::Data::SpotLight* val)
				{
					//임시 리스트에서 찾았다는 얘기 -> 그러니까, 밑의 Erase-Remove_If Idiom에서 찾은 애들만 지우라는 말!
					return std::ranges::find(tEnteredSpotLights, val) != tEnteredSpotLights.end();
				};
			_sceneInfoList->_spotLightList.erase(std::remove_if(_sceneInfoList->_spotLightList.begin(),
				_sceneInfoList->_spotLightList.end(), tIsSpotAlready), _sceneInfoList->_spotLightList.end());
		}
		{
			auto tIsPointAlready = [&](Pg::Data::PointLight* val)
				{
					//임시 리스트에서 찾았다는 얘기 -> 그러니까, 밑의 Erase-Remove_If Idiom에서 찾은 애들만 지우라는 말!
					return std::ranges::find(tEnteredPointLights, val) != tEnteredPointLights.end();
				};
			_sceneInfoList->_pointLightList.erase(std::remove_if(_sceneInfoList->_pointLightList.begin(),
				_sceneInfoList->_pointLightList.end(), tIsPointAlready), _sceneInfoList->_pointLightList.end());
		}

		//원본 리스트에서 삭제를 했음.
		//정렬만 해주면 끝.
		SortSceneInfoLightsVector();
	}

	void GraphicsSceneParser::SortSceneInfoLightsVector()
	{
		if (!(_sceneInfoList->_dirLightList.empty()))
		{
			std::sort(_sceneInfoList->_dirLightList.begin(), _sceneInfoList->_dirLightList.end(),
				[](Pg::Data::DirectionalLight* a, Pg::Data::DirectionalLight* b) {return a > b; });
		}

		if (!(_sceneInfoList->_spotLightList.empty()))
		{
			std::sort(_sceneInfoList->_spotLightList.begin(), _sceneInfoList->_spotLightList.end(),
				[](Pg::Data::SpotLight* a, Pg::Data::SpotLight* b) {return a > b; });
		}

		if (!(_sceneInfoList->_pointLightList.empty()))
		{
			std::sort(_sceneInfoList->_pointLightList.begin(), _sceneInfoList->_pointLightList.end(),
				[](Pg::Data::PointLight* a, Pg::Data::PointLight* b) {return a > b; });
		}
	}

	void GraphicsSceneParser::RemapMaterialIdAll()
	{
		//실제로 이제 존재하는 모든 Material에 ID를 새로 부여.
		Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetCombinedLoader()->RemapMaterialIdAll();
	}

	void GraphicsSceneParser::RemapAppendedMatID()
	{
		//실제로 이제 존재하는 모든 Material에 ID를 새로 부여.
		Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetCombinedLoader()->RemapAppendedMatID();
	}

}