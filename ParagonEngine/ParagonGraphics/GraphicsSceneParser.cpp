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

		ClearMakeObjectLists();

		PlacePathsFromName(newScene);
		CheckForPathNameErrors(newScene);

		ExtractMaterialPaths(newScene);
		SyncRenderObjects(newScene);
		RemapMaterialIDs();
		SetupPrimitiveWireframeObjects();
		SyncSceneInformation(newScene);
		BindAdequateFunctions(newScene);
		//이제 별도로 렌더링과 관련된 오브젝트들을 받아야 한다.

		CreateObjMatBuffersAll();

		//실제 리소스를 사용해야 하기에, Initialize에서 현재 호출하고 있지 않음.
		PlaceCubemapList();

		{
			auto tMatVec = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
			assert("");
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

	void GraphicsSceneParser::ClearMakeObjectLists()
	{
		//기존의 직접적 RenderObject 리스트들 클리어. + SceneInformationList.
		_renderObject2DList.reset(new RenderObject2DList());
		_renderObject3DList.reset(new RenderObject3DList());
		_cubeMapList.reset(new RenderObjectCubemapList());
		_primObjectList.reset(new RenderObjectWireframeList());
		_sceneInfoList.reset(new SceneInformationList());
	}

	void GraphicsSceneParser::PlacePathsFromName(const Pg::Data::Scene* const newScene)
	{
		using Pg::Graphics::Helper::GraphicsResourceHelper;
		using Pg::Data::Enums::eAssetDefine;

		PG_WARN("Mesh File Path가 비어있을 때만 Unreal 컨버터가 작동할 것. 유의.");

		for (auto& tGameObject : newScene->GetObjectList())
		{
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
					continue;
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

		assert("");
	}

	void GraphicsSceneParser::CheckForPathNameErrors(const Pg::Data::Scene* const newScene)
	{
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		for (auto& tGameObject : newScene->GetObjectList())
		{
			// RenderObject
			Pg::Data::BaseRenderer* tBaseRenderer = tGameObject->GetComponent<Pg::Data::BaseRenderer>();

			if (tBaseRenderer != nullptr && GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 1)
			{
				Pg::Data::RendererBase3D* tBaseR3D = static_cast<Pg::Data::RendererBase3D*>(tBaseRenderer);
				//이름<->경로 연동 더블 체크.
				tBaseR3D->CheckForPathNameErrors();
			}
		}
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

					//만약 비어있지 않다면 (비어있으면 Default Material)
					if (!tBaseR3D->GetMaterialFilePath().empty())
					{
						tMaterialPathSet.insert(tBaseR3D->GetMaterialFilePath());
					}
				}
			}
		}

		//2. set의 값을 RenderObject3DList의 Vector로 옮기기. (중복을 없앤 상태.)
		for (auto& it : tMaterialPathSet)
		{
			auto res = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(it, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);
			RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(res.get());
			//일단은 Default Material ID를 설정해주기.
			_renderObject3DList->_materialPathSet.push_back(std::make_pair(it, tRenderMat));
		}

		//ID는 이제 상관 없다. 따로 일괄적으로 부여하기 때문에.
		//기록하는 방식을 String -> RenderMat 포인터로 변경!
		
		//3. Material Parser에 의해 부여된 MaterialID를 찾아서 순서에 맞게 기록하기. (Material Path Set)
		//auto tMatVec = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
		//for (auto& it : _renderObject3DList->_materialPathSet)
		//{
		//	//같은 것 찾기.
		//	auto res = std::find_if(tMatVec.begin(), tMatVec.end(),
		//		[&it](const std::shared_ptr<Pg::Data::Resources::GraphicsResource>& val)
		//		-> bool {return (it.first.compare(val->GetFilePath()) == 0); });
		//
		//	assert(res != tMatVec.end() && "반드시 해당되는 Material을 여기서 찾았어야 한다.");
		//
		//	RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(res->get());
		//	unsigned int tMatID = tRenderMat->GetID();
		//	it.second = tMatID;
		//}

		//4. unordered_map (_renderObject3DList->_list) 세팅.
		for (auto& it : _renderObject3DList->_materialPathSet)
		{
			_renderObject3DList->_staticList.insert_or_assign(it.second, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject3D>>>>());
			_renderObject3DList->_skinnedList.insert_or_assign(it.second, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject3D>>>>());
		}

		//이때까지는 실제로 명시적으로 지정된 Material만 반영이 된다.
	}

	void GraphicsSceneParser::SyncRenderObjects(const Pg::Data::Scene* const newScene)
	{
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//새로운 씬을 로드하니, 자체적인 Object Count를 리셋한다.
		this->_objectId3dCount = 1;

		//3. 이제 실제 오브젝트 내부 RenderObject 연동.
		for (auto& tGameObject : newScene->GetObjectList())
		{
			auto tComponentVector = tGameObject->GetComponents<Pg::Data::BaseRenderer>();
			if (tComponentVector.empty())
			{
				continue;
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

						_renderObject3DList->_staticList.try_emplace(tRenderMat, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject3D>>>>());
						_renderObject3DList->_skinnedList.try_emplace(tRenderMat, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject3D>>>>());

						//일단은 Default Material ID를 설정해주기.
						_renderObject3DList->_materialPathSet.push_back(std::make_pair(tDefaultMatInstName, tRenderMat));

						//일괄적 부여를 위해 Material 포인터 부여.
						tMaterialInput = tRenderMat;

						//로직 후 디버깅.
						std::string tMsg = tGameObject->GetName();
						tMsg += " : 디폴트 매터리얼 객체 사용됨.";

						//나머지 로직과 호환되게 변수 재할당. 
						tMatPth = tDefaultMatInstName;

						PG_TRACE(tMsg.c_str());
					}
					else
					{
						tMaterialInput = it->second;
					}

					//3D
					//StaticMeshRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
					{
						_renderObject3DList->_staticList.at(tMaterialInput)->push_back(std::make_pair(tGameObject,
							std::make_unique<RenderObjectStaticMesh3D>(tBaseRenderer, _objectId3dCount)));
					}
					//SkinnedMeshRenderer
					else if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
					{
						_renderObject3DList->_skinnedList.at(tMaterialInput)->push_back(std::make_pair(tGameObject,
							std::make_unique<RenderObjectSkinnedMesh3D>(tBaseRenderer, _objectId3dCount)));
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
						_renderObject2DList->_list.push_back(std::make_pair(tGameObject,
							std::make_unique<RenderObjectText2D>(tBaseRenderer)));
					}

					//ImageRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::ImageRenderer*).name())) == 0)
					{
						_renderObject2DList->_list.push_back(std::make_pair(tGameObject,
							std::make_unique<RenderObjectImage2D>(tBaseRenderer)));
					}
				}
			}
		}
		assert("");
	}

	void GraphicsSceneParser::RemapMaterialIDs()
	{
		//실제로 이제 존재하는 모든 Material에 ID를 새로 부여.
		Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetCombinedLoader()->RemapMaterialIDs();

		//이를 Static/Skinned List에 반영!

		//Material Path. (first)
		for (auto& it : _renderObject3DList->_staticList)
		{
			//일단은 자기 자신이 속한 Material ID를 부여해줘야 한다.
			RenderMaterial* tRenderMat = it.first;

			for (auto& itt : *(it.second))
			{
				//Material ID를 일괄적으로 부여.
				itt.second->SetMaterialID(tRenderMat->GetID());
			}
		}

		//Material Path. (first)
		for (auto& it : _renderObject3DList->_skinnedList)
		{
			//일단은 자기 자신이 속한 Material ID를 부여해줘야 한다.
			RenderMaterial* tRenderMat = it.first;

			for (auto& itt : *(it.second))
			{
				//Material ID를 일괄적으로 부여.
				itt.second->SetMaterialID(tRenderMat->GetID());
			}
		}

		//auto tMatVec = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
		//assert("");
	}

	void GraphicsSceneParser::SyncSceneInformation(const Pg::Data::Scene* const newScene)
	{
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//Light들의 Component 리스트 일단은 연동.
		for (auto& tGameObject : newScene->GetObjectList())
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

		//Intensity를 기반으로 Sort. ( '>' Operator Overloading )
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

	void GraphicsSceneParser::BindAdequateFunctions(const Pg::Data::Scene* const newScene)
	{
		using Pg::Graphics::Helper::GraphicsResourceHelper;


		for (auto& it : _renderObject3DList->_skinnedList)
		{
			for (auto& [go, ro] : *(it.second.get()))
			{
				RenderObjectSkinnedMesh3D* tSkinnedRO = static_cast<RenderObjectSkinnedMesh3D*>(ro.get());
				Pg::Data::SkinnedMeshRenderer* tSkinnedRenderer = static_cast<Pg::Data::SkinnedMeshRenderer*>(tSkinnedRO->GetBaseRenderer());
				//std::bind로 Data쪽에서 원격으로 함수를 호출할 수 있게.
				tSkinnedRenderer->_setAnimationFunction = std::bind(&RenderObjectSkinnedMesh3D::SetAnimation, tSkinnedRO, std::placeholders::_1);

				//SetAnimation Function Bind.
				//std::function<void(const std::string&)> tSetAnimFunction = [tSkinnedRO](const std::string& animName) {
				//	tSkinnedRO->SetAnimation(animName);
				//};
				//tSkinnedRenderer->_setAnimationFunction = tSetAnimFunction;
			}
		}
	}

	void GraphicsSceneParser::CreateObjMatBuffersAll()
	{
		//모든 오브젝트 렌더링.
		for (auto& it : _renderObject3DList->_staticList)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				it.second->at(i).second->CreateObjMatBuffers();
			}
		}

		//모든 오브젝트 렌더링.
		for (auto& it : _renderObject3DList->_skinnedList)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				it.second->at(i).second->CreateObjMatBuffers();
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



}