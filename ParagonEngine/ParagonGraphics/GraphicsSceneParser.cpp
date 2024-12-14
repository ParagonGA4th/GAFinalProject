#include "GraphicsSceneParser.h"

#include "LayoutDefine.h"
#include "RenderObjectLight.h"
#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "AssetCombinedLoader.h"
#include "AssetBasic3DLoader.h"

//�������� ���� ������Ʈ���� ����Ʈ.
#include "RenderObjectStaticMesh3D.h"
#include "RenderObjectSkinnedMesh3D.h"
#include "RenderObjectInstancedMesh3D.h"
#include "RenderObjectText2D.h"
#include "RenderObjectImage2D.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/LightType.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonUtil/Log.h"

//Wireframe RenderObject��.
#include "Axis.h"
#include "Grid.h"

//�������� ���������� ����Ʈ.
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
		//�̰� �ϴ��� ���������� ������Ʈ�� Save�س��� �� ���̴�.

		//Scene�� �Ľ��ؼ�, ���� �����Ǿ�� �ϴ� Object�� �����Ѵ�.
		//���߿� ���� ���� �����ϴ� �߿� ������Ʈ�� �� �ϳ��� �������� �����ٰų� 
		//��Ȳ�� ���� ���� ����. ���߿� _rendererChangeList�� Ȱ���ϸ� �ȴ�!
		std::string tSceneName = newScene->GetSceneNameConst();
		if (_graphicsObjectsListContainer.contains(tSceneName))
		{
			//���� ���ԵǾ� ������, �״�� �װɷ� ����Ī.
			//Current List Set �Ҵ�.
			_currentListSet = _graphicsObjectsListContainer.at(tSceneName).get();

			//Early Return.
			assert((_currentListSet != nullptr) && "������ �־�� ������");
			return;
		}

		//���ο� ��ü �����.
		//������ ������ RenderObject ����Ʈ�� Ŭ����. + SceneInformationList.
		auto tIt = _graphicsObjectsListContainer.insert(std::make_pair(tSceneName, std::make_unique<GraphicObjectListSet>()));
		//���� List Set �Ҵ�.
		_currentListSet = tIt.first->second.get();
		assert((_currentListSet != nullptr) && "������ �־�� ������");

		ResetParser();

		PlacePathsFromName(newScene);
		CheckForPathNameErrors(newScene);

		ExtractMaterialPaths(newScene);
		SyncRenderObjects(newScene); //����Ʈ ���͸����� ������ִ� ���� ���� �Ѵ�.
		RemapMaterialIdForUninitMaterials();
		CreateAllInstancedRenderObjects(newScene); //Default Material ���� + MaterialID Remapping + SyncRenderObject �� ������, ������ Instanced RenderObjects �����.
		//SetupPrimitiveWireframeObjects(newScene);
		SyncSceneAllLights(newScene);
		CheckBindAdequateFunctions();
		//���� ������ �������� ���õ� ������Ʈ���� �޾ƾ� �Ѵ�.

		CheckCreateObjMatBuffersAll();

		//���� ���ҽ��� ����ؾ� �ϱ⿡, Initialize���� ���� ȣ���ϰ� ���� ����.
		PlaceCubemapList();

		//�ϴ� ��.
		//auto tMatVec = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
		//assert("");
	}

	void GraphicsSceneParser::HandleRenderObjectsRuntime()
	{
		///�̸� �ε�Ǿ��� ������Ʈ�� ���ַ��� �� ���, Delete�� ȣ����� �ʴ´�.
		///Modify�� ���ڱ� Add Logic�� �߻���Ų��.
		///���� �߻� ��� : test_colorCorrect���� ������Ʈ ��ü�� �����Ϸ��� �� ��.
		///ó������ ������ ������Ʈ���� ���� �ȵǴ� ���ΰ�?

		//CheckCreate�� �������� �Ľ� ����� ���.
		//�̸� üũ�ؼ�, EarlyReturn �����ϸ� �ϱ�!
		if (_runtimeAddedObjectList.empty() &&
			_runtimeModifiedObjectList.empty() &&
			_runtimeDeletedObjectList.empty())
		{
			return;
		}

		assert(false && "������� �� �Լ��� ������� �ʱ�� �ߴ�");

		//Added Objects : �̸� PlaceCorrectPath & Error Check.
		std::for_each(_runtimeAddedObjectList.begin(), _runtimeAddedObjectList.end(), [this](Pg::Data::GameObject*& it)
			{
				PlaceCorrectPathSingleRenderer(it);
				CheckPathNameErrorSingleRenderer(it);
			});

		//Modified Objects : �̸� PlaceCorrectPath & Error Check.
		std::for_each(_runtimeModifiedObjectList.begin(), _runtimeModifiedObjectList.end(), [this](Pg::Data::GameObject*& it)
			{
				PlaceCorrectPathSingleRenderer(it);
				CheckPathNameErrorSingleRenderer(it);
			});

		//�̸� Material�� �߰� / �����ؾ� �Ѵٸ�, �̸� ���� �޾ƾ� �Ѵ�.
		RuntimeExtractMaterialPathsVector(&_runtimeAddedObjectList);
		RuntimeExtractMaterialPathsVector(&_runtimeModifiedObjectList);

		//Skinned�� ���, BindAdequateFunctions ���� ȣ��Ǿ�� �Ѵ�!
		//Add ó��.
		//������ ObjectBuffer ������ ��.-> Toggle�ؾ� (�⺻������ �Ǿ� ����)

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

		//�߰��� �͸� �˻��ؼ� �����!
		RemapAppendedMatID();

		//Light ���õ� �ϰ��� ����.
		RuntimeAddLightsVector(&_runtimeAddedObjectList);
		RuntimeModifyLightsVector(&_runtimeModifiedObjectList);
		RuntimeDeleteLightsVector(&_runtimeDeletedObjectList);


		//EarlyReturn�� �ȵǾ��ٴ� �� ���� �ٲܰ� �ִٴ� ��.
		//SceneParser ���ο� �߰��� / ����� ������Ʈ�� ������ ȣ������.
		//�̸� �޾Ƽ� �ϸ� ���� ���� �� ����.
		CheckBindAdequateFunctions();
		CheckCreateObjMatBuffersAll();

		///TODO : �׸��� �߰��� Material�� ���Ӱ� �߰ߵǸ� �̸� �ش� �����鸸 �����ؼ� MaterialPath�� ���� ����� �־���� �Ѵ�.
		///

		//�ϰ������� Clear & Update.
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
		return _currentListSet->_renderObject2DList.get();
	}

	Pg::Graphics::RenderObject3DList* GraphicsSceneParser::GetRenderObject3DList()
	{
		return _currentListSet->_renderObject3DList.get();
	}

	Pg::Graphics::RenderObjectCubemapList* GraphicsSceneParser::GetRenderObjectCubemapList()
	{
		return _currentListSet->_cubeMapList.get();
	}

	Pg::Graphics::RenderObjectWireframeList* GraphicsSceneParser::GetRenderObjectWireframeList()
	{
		return _currentListSet->_primObjectList.get();
	}

	Pg::Graphics::SceneInformationList* GraphicsSceneParser::GetSceneInformationList()
	{
		return _currentListSet->_sceneInfoList.get();
	}

	void GraphicsSceneParser::SetupPrimitiveWireframeObjects(const Pg::Data::Scene* const newScene)
	{
		if (!newScene->GetIs3D())
		{
			return;
		}

		// Primitive RenderObject ���� + Initialize();
		_currentListSet->_primObjectList->_list.push_back(std::make_unique<Grid>());
		_currentListSet->_primObjectList->_list.push_back(std::make_unique<Axis>());

		//�ϰ������� BuildBuffers ����.
		for (auto& it : _currentListSet->_primObjectList->_list)
		{
			it->BuildBuffers();
		}
	}

	void GraphicsSceneParser::PlaceCubemapList()
	{
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Data::Enums::eAssetDefine;

		//Index : 1 �߰�.
		{
			//Cubemap �����͸� �ޱ�.
			auto tCubemapData = GraphicsResourceManager::Instance()->GetResource(Pg::Defines::ASSET_DEFAULT_ENVIRONMENT_CUBEMAP_PATH, eAssetDefine::_CUBEMAP);
			_currentListSet->_cubeMapList->_list.push_back(static_cast<RenderCubemap*>(tCubemapData.get()));
		}
	}

	void GraphicsSceneParser::ResetParser()
	{

		//���ο� ���� �ε��ϴ�, ��ü���� Object Count�� �����Ѵ�.
		//���� �ٲ�� �������� ObjectId3dCount�� �ٲ��� ���� ��.
		this->_objectId3dCount = 1;

		//Material�� InitState�� ���� �� None���� �ο�. -> �̴� ����Ǿ� ���� Initialize�� State�� �ƴ� ������ �۵�.
		Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetCombinedLoader()->ResetAllKnownMatInitStates();

	}

	void GraphicsSceneParser::PlacePathsFromName(const Pg::Data::Scene* const newScene)
	{
		PG_WARN("Mesh File Path�� ������� ���� Unreal �����Ͱ� �۵��� ��. ����.");
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
		//������Ʈ ���������� -> �ڽ��� � Ÿ������ Renderer���� ����. ���������� ȣ��.
		UINT tAddedDefaultMatCount;
		RuntimeExtractMaterialPathsVector(&(newScene->GetObjectList()));
	}

	void GraphicsSceneParser::SyncRenderObjects(const Pg::Data::Scene* const newScene)
	{
		//3. ���� ���� ������Ʈ ���� RenderObject ����.
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
			assert(tBaseRenderer != nullptr && "�� ���������� �ݵ�� �־�� �Ѵ�.");

			//������ ���⿡ Active������ �˻��ؾ� �Ѵ�.
			if (GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 1)
			{
				//�̸� Material Path�� ���� �ִ� RendererBase3D()���� �����ͷ� ���� ����.
				Pg::Data::RendererBase3D* tBaseR3D = static_cast<Pg::Data::RendererBase3D*>(tBaseRenderer);
				std::string tMatPth = tBaseR3D->GetMaterialFilePath();

				//Material Path Set�� RenderObject3DList���� ã��(Index), ������ ���� ����.
				//auto it = std::find(_renderObject3DList->_materialPathSet.begin(), _renderObject3DList->_materialPathSet.end(), tMatPth);

				auto it = std::find_if(_currentListSet->_renderObject3DList->_materialPathSet.begin(), _currentListSet->_renderObject3DList->_materialPathSet.end(),
					[&tMatPth](const std::pair<std::string, RenderMaterial*>& val)
					-> bool {return (val.first == tMatPth); });
				//�� �� Material ID;

				//unsigned int tMaterialID = NULL;
				RenderMaterial* tMaterialInput = nullptr;

				//�� ã������, Default Material�� ���� �־��ش�.
				if (it == _currentListSet->_renderObject3DList->_materialPathSet.end())
				{
					//CreateDefaultMaterialInstance;
					std::filesystem::path tTempMeshPath = tBaseR3D->GetMeshFilePath();
					std::string tTempMeshName = tTempMeshPath.filename().string();
					std::string tDefaultMatInstName = Pg::Graphics::Helper::GraphicsResourceHelper::GetDefaultMaterialNameFromMeshName(tTempMeshName);

					if (!(Pg::Graphics::Manager::GraphicsResourceManager::Instance()->IsExistDefaultMaterialByMeshName(tTempMeshName)))
					{
						//MeshName���� ������� Default Material�� ���� ����.

						//DefaultMaterial �ε��� ���̴�. "DefaultMaterial_"�� ����ֱ� ������, ����Ʈ�� �ε�� ���̴�.
						Pg::Graphics::Manager::GraphicsResourceManager::Instance()->LoadResource(tDefaultMatInstName, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);

						//��ü �����Ͽ� ���� �ִٰ� ���. (Graphics���� �˻��߱� ������, AssetManager�� �������)
						Pg::Graphics::Manager::GraphicsResourceManager::Instance()->AddSecondaryResource(tDefaultMatInstName, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);
					}

					//�� ����� ����� ���� ����, ���� ��쿡�� �ε常 �ϰ� �ϰ������� insert/assign�ϴ� ������ ����.
					//������, vector ��Ͽ� �߰������.
					// => ��� �Ǿ���, Skinned/Static�� ���� ���̴� �־�!
					auto res = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(tDefaultMatInstName, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);
					RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(res.get());

					//��Ÿ�ӿ��� ������Ʈ�� �Ľ����ָ� ������ִ� Ư����, => ������ �ִ��� üũ�ؾ�. 
					//�̹� ������ �ÿ��� �־��ָ� �ȵ�.
					//������ ���ο� ���͸� ������ ����. (insert_or_assign���� TryEmplace�� ����)

					_currentListSet->_renderObject3DList->_staticList.try_emplace(tRenderMat, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectStaticMesh3D>>>>());
					_currentListSet->_renderObject3DList->_skinnedList.try_emplace(tRenderMat, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectSkinnedMesh3D>>>>());

					//�ϴ��� Default Material ID�� �������ֱ�.
					_currentListSet->_renderObject3DList->_materialPathSet.try_emplace(tDefaultMatInstName, tRenderMat);

					//�ϰ��� �ο��� ���� Material ������ �ο�.
					tMaterialInput = tRenderMat;

					//���� �� �����.
					std::string tMsg = obj->GetName();
					tMsg += " : ����Ʈ ���͸��� ��ü ����.";

					//������ ������ ȣȯ�ǰ� ���� ���Ҵ�. 
					tMatPth = tDefaultMatInstName;

					//PG_TRACE(tMsg.c_str());
				}
				else
				{
					tMaterialInput = it->second;
				}

				//MatPth���� ��� ���� ��������, 
				//���� isInstanced�� ���� ���� ���:
				//���⼭ ����Ǵ� ���� ���ƾ� �Ѵ�. 
				//�ѹ� �� �����ٰ� �����ؾ�! �̶��� ��� DefaultMaterial���� ��� �� �ε�Ǿ��� �����̱� ������.
				if (tBaseR3D->GetIsInstanced())
				{
					//���� �ν��Ͻ��� ���Ǵ� Renderer��� ���⼭ ó������ �ʴ´�!
					//PG_TRACE("IS_INSTANCED");
					break;
				}

				//3D
				//���ĺ����� ����ϴ� ���
				if (tMaterialInput->GetIsUseAlphaBlending())
				{
					//StaticMeshRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
					{
						auto tUPtr = std::make_unique<AlphaBlendedTuple>(obj, tMaterialInput, false);
						tUPtr->_eitherStaticMesh = std::make_unique<RenderObjectStaticMesh3D>(tBaseRenderer, _objectId3dCount);
						tUPtr->_eitherStaticMesh->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));

						_currentListSet->_renderObject3DList->_allAlphaBlendedList.push_back(std::move(tUPtr));
					}
					//SkinnedMeshRenderer
					else if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
					{
						auto tUPtr = std::make_unique<AlphaBlendedTuple>(obj, tMaterialInput, true);
						tUPtr->_eitherSkinnedMesh = std::make_unique<RenderObjectSkinnedMesh3D>(tBaseRenderer, _objectId3dCount);
						tUPtr->_eitherSkinnedMesh->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));

						_currentListSet->_renderObject3DList->_allAlphaBlendedList.push_back(std::move(tUPtr));
					}
				}
				else //�׳� ������ ������.
				{
					//StaticMeshRenderer
					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
					{
						_currentListSet->_renderObject3DList->_staticList.at(tMaterialInput)->push_back(std::make_pair(obj,
							std::make_unique<RenderObjectStaticMesh3D>(tBaseRenderer, _objectId3dCount)));

						//���������� MaterialID �Ҵ�. ���̶��� ������ ���̴�.
						_currentListSet->_renderObject3DList->_staticList.at(tMaterialInput)->back().second->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));
					}
					//SkinnedMeshRenderer
					else if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
					{
						_currentListSet->_renderObject3DList->_skinnedList.at(tMaterialInput)->push_back(std::make_pair(obj,
							std::make_unique<RenderObjectSkinnedMesh3D>(tBaseRenderer, _objectId3dCount)));

						//���������� MaterialID �Ҵ�. ���̶��� ������ ���̴�.
						_currentListSet->_renderObject3DList->_skinnedList.at(tMaterialInput)->back().second->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));
					}
				}

				//ObjectId3d�� ��ġ�� �ʵ��� ++
				_objectId3dCount++;
			}
			else if (GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 0)
			{
				//2D
				//TextRenderer
				Pg::Data::RendererBase2D* t2DBase = static_cast<Pg::Data::RendererBase2D*>(tBaseRenderer);

				if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::TextRenderer*).name())) == 0)
				{
					_currentListSet->_renderObject2DList->_list.push_back(std::make_pair(obj,
						std::make_unique<RenderObjectText2D>(tBaseRenderer)));
				}

				//ImageRenderer
				if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::ImageRenderer*).name())) == 0)
				{
					_currentListSet->_renderObject2DList->_list.push_back(std::make_pair(obj,
						std::make_unique<RenderObjectImage2D>(tBaseRenderer)));

					RenderObjectImage2D* tTemp = static_cast<RenderObjectImage2D*>(_currentListSet->_renderObject2DList->_list.back().second.get());
					t2DBase->_width = *(tTemp->_imageWidth);
					t2DBase->_height = *(tTemp->_imageHeight);
				}
			}
		}
	}

	void GraphicsSceneParser::CreateAllInstancedRenderObjects(const Pg::Data::Scene* const newScene)
	{
		//Opaque�� ȣȯ��.
		for (auto& obj : newScene->GetObjectList())
		{
			using Pg::Graphics::Helper::GraphicsResourceHelper;

			auto tComponentVector = obj->GetComponents<Pg::Data::BaseRenderer>();

			if (tComponentVector.empty())
			{
				continue;
			}
			for (int i = 0; i < tComponentVector.size(); i++)
			{
				// RenderObject
				Pg::Data::BaseRenderer* tBaseRenderer = tComponentVector.at(i);
				assert(tBaseRenderer != nullptr && "�� ���������� �ݵ�� �־�� �Ѵ�.");

				//3D��
				if (GraphicsResourceHelper::IsRenderer3D(tBaseRenderer->GetRendererTypeName()) == 1)
				{
					//�̸� Material Path�� ���� �ִ� RendererBase3D()���� �����ͷ� ���� ����.
					Pg::Data::RendererBase3D* tBaseR3D = static_cast<Pg::Data::RendererBase3D*>(tBaseRenderer);

					//isInstanced�� �ش�� ��츸 ���⼭ �����ؾ�.
					if (!tBaseR3D->GetIsInstanced())
					{
						continue;
					}

					std::string tMatPth = tBaseR3D->GetMaterialFilePath();

					std::filesystem::path tTempMeshPath = tBaseR3D->GetMeshFilePath();
					std::string tTempMeshName = tTempMeshPath.filename().string();
					RenderMaterial* tMaterialInput = nullptr;

					if (tMatPth.empty())
					{
						//Default Material.
						std::string tDefaultMatInstName = Pg::Graphics::Helper::GraphicsResourceHelper::GetDefaultMaterialNameFromMeshName(tTempMeshName);
						auto res = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(tDefaultMatInstName, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);
						tMaterialInput = static_cast<RenderMaterial*>(res.get());

						//�ϴ��� Default Material ID�� �������ֱ�.
						//_currentListSet->_renderObject3DList->_materialPathSet.try_emplace(tDefaultMatInstName, tMaterialInput);
					}
					else
					{
						//���� PgShaderParser���� ������� Material.
						auto res = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(tMatPth, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);
						tMaterialInput = static_cast<RenderMaterial*>(res.get());
					}

					//������ RenderMaterial�� �� ���� ��.
					auto tRes = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(tBaseR3D->GetMeshFilePath(), Pg::Data::Enums::eAssetDefine::_3DMODEL);
					Asset3DModelData* modelData = static_cast<Asset3DModelData*>(tRes.get());
					//������ �ְ�, ������ �����ϰ�.
					_currentListSet->_renderObject3DList->_instancedStaticList.try_emplace(modelData, std::make_unique<BufferInstancedPairList>());
					_currentListSet->_renderObject3DList->_instancedCulledOppositeStaticList.try_emplace(modelData, std::make_unique<BufferInstancedPairList>());
					_currentListSet->_renderObject3DList->_instancedStaticAlphaClippedList.try_emplace(modelData, std::make_unique<BufferInstancedPairList>());
					_currentListSet->_renderObject3DList->_instancedCulledOppositeStaticAlphaClippedList.try_emplace(modelData, std::make_unique<BufferInstancedPairList>());

					if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
					{
						bool tIsAlphaClipped = modelData->_isUseAlphaClipping;
						bool tIsScaleOddMinus = tBaseRenderer->_object->_transform.IsScaleOddMinus();
						//Culling + Alpha Clipping - ����� �����ؼ� �����ϱ�.

						if (tIsAlphaClipped && !tIsScaleOddMinus)
						{
							//���� Ŭ������ ����, �Ϲ���.
							auto& tVectorPtr = _currentListSet->_renderObject3DList->_instancedStaticAlphaClippedList.at(modelData)->_instancedStaticPairVec;

							//�� �ֱ�. ����! ID3D1Buffer�� ���� ����. (Instancing�� ����)
							tVectorPtr.push_back(InstancedStaticPair(tMaterialInput, std::make_shared<RenderObjectInstancedMesh3D>(tBaseRenderer, _objectId3dCount)));
							tVectorPtr.back()._instancedRenderObject->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));
						}
						else if (tIsAlphaClipped && tIsScaleOddMinus)
						{
							//���� Ŭ������ ����, �ݴ�� ������.
							auto& tVectorPtr = _currentListSet->_renderObject3DList->_instancedCulledOppositeStaticAlphaClippedList.at(modelData)->_instancedStaticPairVec;

							//�� �ֱ�. ����! ID3D1Buffer�� ���� ����. (Instancing�� ����)
							tVectorPtr.push_back(InstancedStaticPair(tMaterialInput, std::make_shared<RenderObjectInstancedMesh3D>(tBaseRenderer, _objectId3dCount)));
							tVectorPtr.back()._instancedRenderObject->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));
						}
						else if (!tIsAlphaClipped && !tIsScaleOddMinus)
						{
							// ���� Ŭ���� ���� X, �Ϲ���.
							auto& tVectorPtr = _currentListSet->_renderObject3DList->_instancedStaticList.at(modelData)->_instancedStaticPairVec;

							//�� �ֱ�. ����! ID3D1Buffer�� ���� ����. (Instancing�� ����)
							tVectorPtr.push_back(InstancedStaticPair(tMaterialInput, std::make_shared<RenderObjectInstancedMesh3D>(tBaseRenderer, _objectId3dCount)));
							tVectorPtr.back()._instancedRenderObject->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));
						}
						else if (!tIsAlphaClipped && tIsScaleOddMinus)
						{
							// ���� Ŭ���� ���� X, �ݴ�� ������.

							auto& tVectorPtr = _currentListSet->_renderObject3DList->_instancedCulledOppositeStaticList.at(modelData)->_instancedStaticPairVec;

							//�� �ֱ�. ����! ID3D1Buffer�� ���� ����. (Instancing�� ����)
							tVectorPtr.push_back(InstancedStaticPair(tMaterialInput, std::make_shared<RenderObjectInstancedMesh3D>(tBaseRenderer, _objectId3dCount)));
							tVectorPtr.back()._instancedRenderObject->SetMaterialIdPointer(&(tMaterialInput->GetMaterialID()));
						}
					}
					else
					{
						assert(false && "�������� �ʴ� ����. Instanced Skinning �������� �ʴ´�.");
					}

					_objectId3dCount++;
					//ObjectId3d�� ��ġ�� �ʵ��� ++
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


		for (auto& it : _currentListSet->_renderObject3DList->_skinnedList)
		{
			for (auto& [go, ro] : *(it.second.get()))
			{
				RenderObjectSkinnedMesh3D* tSkinnedRO = static_cast<RenderObjectSkinnedMesh3D*>(ro.get());
				Pg::Data::SkinnedMeshRenderer* tSkinnedRenderer = static_cast<Pg::Data::SkinnedMeshRenderer*>(tSkinnedRO->GetBaseRenderer());
				//std::bind�� Data�ʿ��� �������� �Լ��� ȣ���� �� �ְ�.
				//�غ� �ȵǾ��� ����,
				if (!(ro->_isInternalUpToDate))
				{
					tSkinnedRenderer->_setAnimationFunction = std::bind(&RenderObjectSkinnedMesh3D::SetAnimation, tSkinnedRO, std::placeholders::_1, std::placeholders::_2);
					tSkinnedRenderer->_findAnimTransformFunction = std::bind(&RenderObjectSkinnedMesh3D::FindAnimTransform, tSkinnedRO, std::placeholders::_1);
				}
			}
		}

		for (auto& it : _currentListSet->_renderObject3DList->_allAlphaBlendedList)
		{
			if (it->_isSkinned)
			{
				RenderObjectSkinnedMesh3D* tSkinnedRO = static_cast<RenderObjectSkinnedMesh3D*>(it->_eitherSkinnedMesh.get());
				Pg::Data::SkinnedMeshRenderer* tSkinnedRenderer = static_cast<Pg::Data::SkinnedMeshRenderer*>(it->_eitherSkinnedMesh->GetBaseRenderer());

				if (!(it->_eitherSkinnedMesh->_isInternalUpToDate))
				{
					tSkinnedRenderer->_setAnimationFunction = std::bind(&RenderObjectSkinnedMesh3D::SetAnimation, tSkinnedRO, std::placeholders::_1, std::placeholders::_2);
					tSkinnedRenderer->_findAnimTransformFunction = std::bind(&RenderObjectSkinnedMesh3D::FindAnimTransform, tSkinnedRO, std::placeholders::_1);
				}
			}
		}


	}

	void GraphicsSceneParser::CheckCreateObjMatBuffersAll()
	{
		//��� ������Ʈ ������. (static)
		for (auto& it : _currentListSet->_renderObject3DList->_staticList)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				//ObjectMaterial Buffer�� ��������� ���� �ֵ鸸 ȣ��, ����.
				if (!(it.second->at(i).second->_isInternalUpToDate))
				{
					it.second->at(i).second->CreateObjMatBuffers();
					//���� ��.
					it.second->at(i).second->_isInternalUpToDate = true;
				}
			}
		}

		//��� ������Ʈ ������. (skinned)
		for (auto& it : _currentListSet->_renderObject3DList->_skinnedList)
		{
			//Vector
			for (int i = 0; i < it.second->size(); i++)
			{
				//ObjectMaterial Buffer�� ��������� ���� �ֵ鸸 ȣ��, ����.
				if (!(it.second->at(i).second->_isInternalUpToDate))
				{
					it.second->at(i).second->CreateObjMatBuffers();
					//���� ��.
					it.second->at(i).second->_isInternalUpToDate = true;
				}
			}
		}

		//Instanced ��ü �߰��ؾ� �Ѵ�. �۵� ����� �Ϻ� �ٸ�����.
		for (auto& [bModelData, bBufferVecPair] : _currentListSet->_renderObject3DList->_instancedStaticList)
		{
			auto& bVecPtr = bBufferVecPair->_instancedStaticPairVec;

			//assert(bVecPtr != nullptr);
			//assert(!bVecPtr.empty());

			if (bVecPtr.empty())
			{
				continue;
			}
			unsigned int tVecVBSize = bVecPtr.size();

			for (int i = 0; i < tVecVBSize; i++)
			{
				if (!(bVecPtr.at(i)._instancedRenderObject->_isInternalUpToDate))
				{
					bVecPtr.at(i)._instancedRenderObject->CreateObjMatBuffers();
					bVecPtr.at(i)._instancedRenderObject->_isInternalUpToDate = true;
				}
			}
		}

		for (auto& [bModelData, bBufferVecPair] : _currentListSet->_renderObject3DList->_instancedCulledOppositeStaticList)
		{
			auto& bVecPtr = bBufferVecPair->_instancedStaticPairVec;

			//assert(bVecPtr != nullptr);
			//assert(!bVecPtr.empty());

			if (bVecPtr.empty())
			{
				continue;
			}

			unsigned int tVecVBSize = bVecPtr.size();

			for (int i = 0; i < tVecVBSize; i++)
			{
				if (!(bVecPtr.at(i)._instancedRenderObject->_isInternalUpToDate))
				{
					bVecPtr.at(i)._instancedRenderObject->CreateObjMatBuffers();
					bVecPtr.at(i)._instancedRenderObject->_isInternalUpToDate = true;
				}
			}
		}

		for (auto& [bModelData, bBufferVecPair] : _currentListSet->_renderObject3DList->_instancedStaticAlphaClippedList)
		{
			auto& bVecPtr = bBufferVecPair->_instancedStaticPairVec;

			//assert(bVecPtr != nullptr);
			//assert(!bVecPtr.empty());

			if (bVecPtr.empty())
			{
				continue;
			}

			unsigned int tVecVBSize = bVecPtr.size();

			for (int i = 0; i < tVecVBSize; i++)
			{
				if (!(bVecPtr.at(i)._instancedRenderObject->_isInternalUpToDate))
				{
					bVecPtr.at(i)._instancedRenderObject->CreateObjMatBuffers();
					bVecPtr.at(i)._instancedRenderObject->_isInternalUpToDate = true;
				}
			}
		}

		for (auto& [bModelData, bBufferVecPair] : _currentListSet->_renderObject3DList->_instancedCulledOppositeStaticAlphaClippedList)
		{
			auto& bVecPtr = bBufferVecPair->_instancedStaticPairVec;

			//assert(bVecPtr != nullptr);
			//assert(!bVecPtr.empty());

			if (bVecPtr.empty())
			{
				continue;
			}

			unsigned int tVecVBSize = bVecPtr.size();

			for (int i = 0; i < tVecVBSize; i++)
			{
				if (!(bVecPtr.at(i)._instancedRenderObject->_isInternalUpToDate))
				{
					bVecPtr.at(i)._instancedRenderObject->CreateObjMatBuffers();
					bVecPtr.at(i)._instancedRenderObject->_isInternalUpToDate = true;
				}
			}
		}

		//���� Instanced �����ؼ� CB���� �ſ��� -> ���� VB ������ �Ѵ�.
		//�������� Instancing Format�� �������� Vector�� ����� ����.
		//���� ����� ���� �ӽ� ����.
		CreateBackFaceInstancedTRSBuffer();
		CreateFrontFaceInstancedTRSBuffer();
		CreateClippedBackFaceInstancedTRSBuffer();
		CreateClippedFrontFaceInstancedTRSBuffer();

		//��ǻ� ForwardRendering�� ����� Alpha Blended Object���� �� ������ �ʿ� ������,
		//���� �Ͽ�ȭ�� ���� �����Ѵ�.
		for (auto& it : _currentListSet->_renderObject3DList->_allAlphaBlendedList)
		{
			if (it->_isSkinned)
			{
				it->_eitherSkinnedMesh->_isInternalUpToDate = true;
			}
			else
			{
				it->_eitherStaticMesh->_isInternalUpToDate = true;
			}
		}
	}

	Pg::Data::GameObject* GraphicsSceneParser::GetObjectWithObjID(unsigned int objID)
	{
		Pg::Data::GameObject* tRet = nullptr;

		//Static List ���� ã��.
		for (auto& [bMatPath, bVectorPtr] : _currentListSet->_renderObject3DList->_staticList)
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

		//Skinned List ���� ã��.
		for (auto& [bMatPath, bVectorPtr] : _currentListSet->_renderObject3DList->_skinnedList)
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

		for (auto& [bModelData, bCollection] : _currentListSet->_renderObject3DList->_instancedStaticList)
		{
			for (auto& it : bCollection->_instancedStaticPairVec)
			{
				if (it._instancedRenderObject->GetObjectID() == objID)
				{
					tRet = it._instancedRenderObject->GetBaseRenderer()->_object;
					goto gtFinished;
				}
			}
		}

		for (auto& [bModelData, bCollection] : _currentListSet->_renderObject3DList->_instancedCulledOppositeStaticList)
		{
			for (auto& it : bCollection->_instancedStaticPairVec)
			{
				if (it._instancedRenderObject->GetObjectID() == objID)
				{
					tRet = it._instancedRenderObject->GetBaseRenderer()->_object;
					goto gtFinished;
				}
			}
		}

		for (auto& it : _currentListSet->_renderObject3DList->_allAlphaBlendedList)
		{
			if (it->_isSkinned)
			{
				if (it->_eitherSkinnedMesh->GetObjectID() == objID)
				{
					tRet = it->_eitherSkinnedMesh->GetBaseRenderer()->_object;
					goto gtFinished;
				}
			}
			else
			{
				if (it->_eitherStaticMesh->GetObjectID() == objID)
				{
					tRet = it->_eitherStaticMesh->GetBaseRenderer()->_object;
					goto gtFinished;
				}
			}
		}


		//�����ϰ� Goto ����� ���Ǵ� ���� : nested loops, in single functions.
	gtFinished:
		assert(tRet != nullptr && "������ Picking�� GameObject�� ã�Ҿ�� �ϴ� �Լ����� ���� ã�� ���ߴ�.");
		//PG_TRACE(tRet->GetName().c_str());
		return tRet;
	}

	unsigned int GraphicsSceneParser::GetObjIDWithObject(const Pg::Data::GameObject* const obj)
	{
		unsigned int tRet = NULL;

		//Static List ���� ã��.
		for (auto& [bMatPath, bVectorPtr] : _currentListSet->_renderObject3DList->_staticList)
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

		//Skinned List ���� ã��.
		for (auto& [bMatPath, bVectorPtr] : _currentListSet->_renderObject3DList->_skinnedList)
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

		for (auto& [bModelData, bCollection] : _currentListSet->_renderObject3DList->_instancedStaticList)
		{
			for (auto& it : bCollection->_instancedStaticPairVec)
			{
				if (it._instancedRenderObject->GetBaseRenderer()->_object == obj)
				{
					tRet = it._instancedRenderObject->GetObjectID();
					goto jobFinished;
				}
			}
		}

		for (auto& [bModelData, bCollection] : _currentListSet->_renderObject3DList->_instancedCulledOppositeStaticList)
		{
			for (auto& it : bCollection->_instancedStaticPairVec)
			{
				if (it._instancedRenderObject->GetBaseRenderer()->_object == obj)
				{
					tRet = it._instancedRenderObject->GetObjectID();
					goto jobFinished;
				}
			}
		}

		for (auto& it : _currentListSet->_renderObject3DList->_allAlphaBlendedList)
		{
			if (it->_isSkinned)
			{
				if (it->_eitherSkinnedMesh->GetBaseRenderer()->_object == obj)
				{
					tRet = it->_eitherSkinnedMesh->GetObjectID();
					goto jobFinished;
				}
			}
			else
			{
				if (it->_eitherStaticMesh->GetBaseRenderer()->_object == obj)
				{
					tRet = it->_eitherStaticMesh->GetObjectID();
					goto jobFinished;
				}
			}
		}

		//�����ϰ� Goto ����� ���Ǵ� ���� : nested loops, in single functions.
	jobFinished:
		//���� tRet�� NULL�̶��, Render���� �ʴ� ���ӿ�����Ʈ�� �����ߴٴ� ��.
		//�ѹ� ���͸��Ǿ���� ������, �ش� ��Ȳ�� �Ұ�.

		///�̶�, �ϴ� ���� ����� ����! �Ұ����� ��Ȳ�� �ܺ������� �����ϰ� �����߱� ����.
		//assert(tRet != NULL && "�Ұ����� ��Ȳ, ���͸��Ǿ� �� ���ӿ�����Ʈ�� ���� ��Ͽ��� �߰�X");

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
		//���� �Ǿ���, �ٽ� ����� �ٷ� ����ų� / �׷��� �ʾƾ� �Ѵ�.

		//�ϴ�, ���� ������Ʈ�� �������� ������ �ִ�������.
		auto renderVec = obj->GetComponents<Pg::Data::BaseRenderer>();
		if (renderVec.empty())
		{
			//�ش� ������Ʈ �ִ� �׿��� -> ������Ʈ�� ������� ���. (������ �ƹ��͵� ���� ����)
			_currentListSet->_renderObject2DList->DeleteRenderObjectWithGameObject(obj);
			_currentListSet->_renderObject3DList->DeleteRenderObjectWithGameObject(obj);

			return;
		}

		auto tRo2d = _currentListSet->_renderObject2DList->GetRenderObjectWithGameObject(obj);
		auto tRo3d = _currentListSet->_renderObject3DList->GetRenderObjectWithGameObject(obj);
		if (tRo2d.empty() && tRo3d.empty())
		{
			// 1. �ش� ������Ʈ ���� �׿��� -> ������Ʈ�� ����� ���.
			//���� ���µ�, ������ ���� ��!
			AddSingleRenderObject(obj);

			return;
		}

		// 2. �ش� ������Ʈ �ִ� �׿��� -> ������Ʈ ���� �����Ǵ� ���.
		//��·�� ����� �ٽ� ������ �Ѵ�.
		_currentListSet->_renderObject2DList->DeleteRenderObjectWithGameObject(obj);
		_currentListSet->_renderObject3DList->DeleteRenderObjectWithGameObject(obj);
		AddSingleRenderObject(obj);

	}

	void GraphicsSceneParser::DeleteSingleRenderObject(Pg::Data::GameObject* obj)
	{
		auto renderVec = obj->GetComponents<Pg::Data::BaseRenderer>();

		//(������ �ƹ��͵� ���� ����)
		_currentListSet->_renderObject2DList->DeleteRenderObjectWithGameObject(obj);
		_currentListSet->_renderObject3DList->DeleteRenderObjectWithGameObject(obj);
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
				//�̹� MeshFilePath�� �ִٴ� ���̴�.
				//���� �ý��� ������ �� ��. 
				//ȣȯ�� ���� �ش� �ڵ� �ۼ�.
				return;
			}

			//�𸮾�� ǥ�� �ٲٱ�. (���� �ش�� ���)
			tBaseR3D->ConvertPotentialUnrealValues();

			//����� Name ������ GraphicsResourceManager���� ��ȣ Ȯ�� :
			//��ü Path ��� + ����.
			if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
			{
				//������ ����.
				Pg::Data::StaticMeshRenderer* tActualRenderer = static_cast<Pg::Data::StaticMeshRenderer*>(tBaseRenderer);

				//���ҽ� �Ŵ������� Ȯ�� -> MeshName / MeshPath.
				//������ �� ���������� �����ؾ� �Ѵ�.
				tActualRenderer->SetMeshFilePath(Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResourcePathByName(
					tActualRenderer->_meshName, eAssetDefine::_3DMODEL));

				//���ҽ� �Ŵ������� Ȯ�� -> MaterialName / MaterialPath
				if (!tActualRenderer->_materialName.empty())
				{
					tActualRenderer->SetMaterialFilePath(Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResourcePathByName(
						tActualRenderer->_materialName, eAssetDefine::_RENDERMATERIAL));
				}
			}
			else if (tBaseRenderer->GetRendererTypeName().compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
			{
				//Skinned�� �����Ϸ��� �ϴ�, ��� �߰��Ǿ�� �Ѵ�.
				//������ ����.
				Pg::Data::SkinnedMeshRenderer* tActualRenderer = static_cast<Pg::Data::SkinnedMeshRenderer*>(tBaseRenderer);

				//���ҽ� �Ŵ������� Ȯ�� -> MeshName / MeshPath.
				//������ �� ���������� �����ؾ� �Ѵ�.
				tActualRenderer->SetMeshFilePath(Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResourcePathByName(
					tActualRenderer->_meshName, eAssetDefine::_3DMODEL));

				//���ҽ� �Ŵ������� Ȯ�� -> MaterialName / MaterialPath
				if (!tActualRenderer->_materialName.empty())
				{
					tActualRenderer->SetMaterialFilePath(Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResourcePathByName(
						tActualRenderer->_materialName, eAssetDefine::_RENDERMATERIAL));
				}
			}
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
			//�̸�<->��� ���� ���� üũ.
			tBaseR3D->CheckForPathNameErrors();
		}
	}

	void GraphicsSceneParser::RuntimeExtractMaterialPathsVector(const std::vector<Pg::Data::GameObject*>* runtimeObjList)
	{
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//1. ���� ������ ������ �ִ� Material Path���� ���� �� �̰�, �⺻���� ������ �����Ѵ�.
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

					//���� ������� �ʴٸ� (��������� Default Material)
					if (!tBaseR3D->GetMaterialFilePath().empty())
					{
						tMaterialPathSet.insert(tBaseR3D->GetMaterialFilePath());
					}
				}
			}
		}

		//2. set�� ���� RenderObject3DList�� Vector�� �ű��. (�ߺ��� ���� ����.)
		//����Ǵ� ���� : �̹� ������ ������ �ȵ�.
		for (auto& it : tMaterialPathSet)
		{
			auto res = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(it, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);
			RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(res.get());
			//�ϴ��� Default Material ID�� �������ֱ�.
			_currentListSet->_renderObject3DList->_materialPathSet.try_emplace(it, tRenderMat);
		}

		//ID�� ���� ��� ����. ���� �ϰ������� �ο��ϱ� ������.
		//����ϴ� ����� String -> RenderMat �����ͷ� ����!

		//3. unordered_map (_renderObject3DList->_list) ����.
		for (auto& it : _currentListSet->_renderObject3DList->_materialPathSet)
		{
			//�߿� : �̹� �����ϸ� ���� ����� �ȵȴ�.
			_currentListSet->_renderObject3DList->_staticList.try_emplace(it.second, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectStaticMesh3D>>>>());
			_currentListSet->_renderObject3DList->_skinnedList.try_emplace(it.second, std::make_unique<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectSkinnedMesh3D>>>>());
		}
	}

	void GraphicsSceneParser::RuntimeAddLightsVector(const std::vector<Pg::Data::GameObject*>* objList)
	{
		//��������� Early Return.
		if (objList->empty())
		{
			return;
		}

		//Light���� Component ����Ʈ �ϴ��� ����.
		for (auto& tGameObject : *objList)
		{
			//1. ����Ʈ�� �ִ��� üũ�Ѵ�.
			auto tLightComponentVector = tGameObject->GetComponents<Pg::Data::Light>();

			for (auto& tSingleLight : tLightComponentVector)
			{
				//Directional Light�� ���.
				Pg::Data::DirectionalLight* tDirLight = dynamic_cast<Pg::Data::DirectionalLight*>(tSingleLight);
				if (tDirLight != nullptr)
				{
					_currentListSet->_sceneInfoList->_dirLightList.push_back(tDirLight);
					continue;
				}

				//Spot Light�� ���.
				Pg::Data::SpotLight* tSpotLight = dynamic_cast<Pg::Data::SpotLight*>(tSingleLight);
				if (tSpotLight != nullptr)
				{
					_currentListSet->_sceneInfoList->_spotLightList.push_back(tSpotLight);
					continue;
				}

				//Point Light�� ���.
				Pg::Data::PointLight* tPointLight = dynamic_cast<Pg::Data::PointLight*>(tSingleLight);
				if (tPointLight != nullptr)
				{
					_currentListSet->_sceneInfoList->_pointLightList.push_back(tPointLight);
					continue;
				}
			}
		}
		//�� �־�����, ��ü Sorting �ٽ�.
		SortSceneInfoLightsVector();
	}

	void GraphicsSceneParser::RuntimeModifyLightsVector(const std::vector<Pg::Data::GameObject*>* objList)
	{
		//��������� Early Return.
		if (objList->empty())
		{
			return;
		}

		///�� 3���� ���.
		/// 1. �ش� ������Ʈ ���� �׿��� -> ������Ʈ�� ����� ���.
		/// 2. �ش� ������Ʈ �ִ� �׿��� -> ������Ʈ�� ������� ���.
		/// 3. �ش� ������Ʈ �ִ� �׿��� -> ������Ʈ�� �����Ǵ� ���.

		std::vector<Pg::Data::DirectionalLight*> tEnteredDirLights;
		std::vector<Pg::Data::SpotLight*> tEnteredSpotLights;
		std::vector<Pg::Data::PointLight*> tEnteredPointLights;

		//Light���� Component ����Ʈ ������ ����.
		for (auto& tGameObject : *objList)
		{
			//1. ����Ʈ�� �ִ��� üũ�Ѵ�.
			auto tLightComponentVector = tGameObject->GetComponents<Pg::Data::Light>();

			for (auto& tSingleLight : tLightComponentVector)
			{
				//Directional Light�� ���.
				Pg::Data::DirectionalLight* tDirLight = dynamic_cast<Pg::Data::DirectionalLight*>(tSingleLight);
				if (tDirLight != nullptr)
				{
					tEnteredDirLights.push_back(tDirLight);
					continue;
				}

				//Spot Light�� ���.
				Pg::Data::SpotLight* tSpotLight = dynamic_cast<Pg::Data::SpotLight*>(tSingleLight);
				if (tSpotLight != nullptr)
				{
					tEnteredSpotLights.push_back(tSpotLight);
					continue;
				}

				//Point Light�� ���.
				Pg::Data::PointLight* tPointLight = dynamic_cast<Pg::Data::PointLight*>(tSingleLight);
				if (tPointLight != nullptr)
				{
					tEnteredPointLights.push_back(tPointLight);
					continue;
				}
			}
		}

		//���� SceneInfo ����Ʈ���� ���� �ȿ� ������ �� �����ϱ�.
		//�̹� �ִ��� üũ. 
		//Filter�ؼ� ��ġ�� ���� ����.
		{
			auto tIsDirAlready = [&](Pg::Data::DirectionalLight* val)
				{
					//�ӽ� ����Ʈ���� ã�Ҵٴ� ��� -> �׷��ϱ�, ���� Erase-Remove_If Idiom���� ã�� �ֵ鸸 ������ ��!
					return std::ranges::find(tEnteredDirLights, val) != tEnteredDirLights.end();
				};
			_currentListSet->_sceneInfoList->_dirLightList.erase(std::remove_if(_currentListSet->_sceneInfoList->_dirLightList.begin(),
				_currentListSet->_sceneInfoList->_dirLightList.end(), tIsDirAlready), _currentListSet->_sceneInfoList->_dirLightList.end());
		}
		{
			auto tIsSpotAlready = [&](Pg::Data::SpotLight* val)
				{
					//�ӽ� ����Ʈ���� ã�Ҵٴ� ��� -> �׷��ϱ�, ���� Erase-Remove_If Idiom���� ã�� �ֵ鸸 ������ ��!
					return std::ranges::find(tEnteredSpotLights, val) != tEnteredSpotLights.end();
				};
			_currentListSet->_sceneInfoList->_spotLightList.erase(std::remove_if(_currentListSet->_sceneInfoList->_spotLightList.begin(),
				_currentListSet->_sceneInfoList->_spotLightList.end(), tIsSpotAlready), _currentListSet->_sceneInfoList->_spotLightList.end());
		}
		{
			auto tIsPointAlready = [&](Pg::Data::PointLight* val)
				{
					//�ӽ� ����Ʈ���� ã�Ҵٴ� ��� -> �׷��ϱ�, ���� Erase-Remove_If Idiom���� ã�� �ֵ鸸 ������ ��!
					return std::ranges::find(tEnteredPointLights, val) != tEnteredPointLights.end();
				};
			_currentListSet->_sceneInfoList->_pointLightList.erase(std::remove_if(_currentListSet->_sceneInfoList->_pointLightList.begin(),
				_currentListSet->_sceneInfoList->_pointLightList.end(), tIsPointAlready), _currentListSet->_sceneInfoList->_pointLightList.end());
		}

		//��ģ�� ������ ������, �ٽ� ���� �ִ´�. (�����̱� ����)
		std::copy(tEnteredDirLights.begin(), tEnteredDirLights.end(), std::back_inserter(_currentListSet->_sceneInfoList->_dirLightList));
		std::copy(tEnteredSpotLights.begin(), tEnteredSpotLights.end(), std::back_inserter(_currentListSet->_sceneInfoList->_spotLightList));
		std::copy(tEnteredPointLights.begin(), tEnteredPointLights.end(), std::back_inserter(_currentListSet->_sceneInfoList->_pointLightList));

		//Data ��ü�� �����ٰ� ���� ������, ���� ���� ������ ������ �ʿ� ����.
		//Intensity�� ������� Sort. ( '>' Operator Overloading )
		//�׳� ������ Sort�� �ٽ� ������ �ȴ�!
		SortSceneInfoLightsVector();
	}

	void GraphicsSceneParser::RuntimeDeleteLightsVector(const std::vector<Pg::Data::GameObject*>* objList)
	{
		//��������� Early Return.
		if (objList->empty())
		{
			return;
		}

		std::vector<Pg::Data::DirectionalLight*> tEnteredDirLights;
		std::vector<Pg::Data::SpotLight*> tEnteredSpotLights;
		std::vector<Pg::Data::PointLight*> tEnteredPointLights;

		//Light���� Component ����Ʈ ������ ����.
		for (auto& tGameObject : *objList)
		{
			//1. ����Ʈ�� �ִ��� üũ�Ѵ�.
			auto tLightComponentVector = tGameObject->GetComponents<Pg::Data::Light>();

			for (auto& tSingleLight : tLightComponentVector)
			{
				//Directional Light�� ���.
				Pg::Data::DirectionalLight* tDirLight = dynamic_cast<Pg::Data::DirectionalLight*>(tSingleLight);
				if (tDirLight != nullptr)
				{
					tEnteredDirLights.push_back(tDirLight);
					continue;
				}

				//Spot Light�� ���.
				Pg::Data::SpotLight* tSpotLight = dynamic_cast<Pg::Data::SpotLight*>(tSingleLight);
				if (tSpotLight != nullptr)
				{
					tEnteredSpotLights.push_back(tSpotLight);
					continue;
				}

				//Point Light�� ���.
				Pg::Data::PointLight* tPointLight = dynamic_cast<Pg::Data::PointLight*>(tSingleLight);
				if (tPointLight != nullptr)
				{
					tEnteredPointLights.push_back(tPointLight);
					continue;
				}
			}
		}

		//���� SceneInfo ����Ʈ���� ���� �ȿ� ������ �� �����ϱ�.
		//�̹� �ִ��� üũ. 
		//Filter�ؼ� ��ġ�� ���� ����.
		{
			auto tIsDirAlready = [&](Pg::Data::DirectionalLight* val)
				{
					//�ӽ� ����Ʈ���� ã�Ҵٴ� ��� -> �׷��ϱ�, ���� Erase-Remove_If Idiom���� ã�� �ֵ鸸 ������ ��!
					return std::ranges::find(tEnteredDirLights, val) != tEnteredDirLights.end();
				};
			_currentListSet->_sceneInfoList->_dirLightList.erase(std::remove_if(_currentListSet->_sceneInfoList->_dirLightList.begin(),
				_currentListSet->_sceneInfoList->_dirLightList.end(), tIsDirAlready), _currentListSet->_sceneInfoList->_dirLightList.end());
		}
		{
			auto tIsSpotAlready = [&](Pg::Data::SpotLight* val)
				{
					//�ӽ� ����Ʈ���� ã�Ҵٴ� ��� -> �׷��ϱ�, ���� Erase-Remove_If Idiom���� ã�� �ֵ鸸 ������ ��!
					return std::ranges::find(tEnteredSpotLights, val) != tEnteredSpotLights.end();
				};
			_currentListSet->_sceneInfoList->_spotLightList.erase(std::remove_if(_currentListSet->_sceneInfoList->_spotLightList.begin(),
				_currentListSet->_sceneInfoList->_spotLightList.end(), tIsSpotAlready), _currentListSet->_sceneInfoList->_spotLightList.end());
		}
		{
			auto tIsPointAlready = [&](Pg::Data::PointLight* val)
				{
					//�ӽ� ����Ʈ���� ã�Ҵٴ� ��� -> �׷��ϱ�, ���� Erase-Remove_If Idiom���� ã�� �ֵ鸸 ������ ��!
					return std::ranges::find(tEnteredPointLights, val) != tEnteredPointLights.end();
				};
			_currentListSet->_sceneInfoList->_pointLightList.erase(std::remove_if(_currentListSet->_sceneInfoList->_pointLightList.begin(),
				_currentListSet->_sceneInfoList->_pointLightList.end(), tIsPointAlready), _currentListSet->_sceneInfoList->_pointLightList.end());
		}

		//���� ����Ʈ���� ������ ����.
		//���ĸ� ���ָ� ��.
		SortSceneInfoLightsVector();
	}

	void GraphicsSceneParser::SortSceneInfoLightsVector()
	{
		if (!(_currentListSet->_sceneInfoList->_dirLightList.empty()))
		{
			std::sort(_currentListSet->_sceneInfoList->_dirLightList.begin(), _currentListSet->_sceneInfoList->_dirLightList.end(),
				[](Pg::Data::DirectionalLight* a, Pg::Data::DirectionalLight* b) {return a > b; });
		}

		if (!(_currentListSet->_sceneInfoList->_spotLightList.empty()))
		{
			std::sort(_currentListSet->_sceneInfoList->_spotLightList.begin(), _currentListSet->_sceneInfoList->_spotLightList.end(),
				[](Pg::Data::SpotLight* a, Pg::Data::SpotLight* b) {return a > b; });
		}

		if (!(_currentListSet->_sceneInfoList->_pointLightList.empty()))
		{
			std::sort(_currentListSet->_sceneInfoList->_pointLightList.begin(), _currentListSet->_sceneInfoList->_pointLightList.end(),
				[](Pg::Data::PointLight* a, Pg::Data::PointLight* b) {return a > b; });
		}
	}

	void GraphicsSceneParser::RemapMaterialIdForUninitMaterials()
	{
		//������ ���� �����ϴ� ��� Material�� ID�� ���� �ο�.
		Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetCombinedLoader()->RemapMaterialIdForUninitMaterials();
	}

	void GraphicsSceneParser::RemapAppendedMatID()
	{
		//������ ���� �����ϴ� ��� Material�� ID�� ���� �ο�.
		Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetCombinedLoader()->RemapAppendedMatID();
	}

	void GraphicsSceneParser::CreateBackFaceInstancedTRSBuffer()
	{
		//�Ϲ����� Backface Culling ���.
		std::vector<std::pair<Asset3DModelData*, std::vector<RenderObjectInstancedMesh3D*>>> tToMakeInstSeparateVec;

		for (auto& [bModelData, bVecPair] : _currentListSet->_renderObject3DList->_instancedStaticList)
		{
			//
			auto& bVecPtr = bVecPair->_instancedStaticPairVec;

			//assert(bVecPtr != nullptr);
			//assert(!bVecPtr.empty());
			if (bVecPtr.empty())
			{
				continue;
			}

			unsigned int tVecVBSize = bVecPtr.size();

			//���� ��� �߰�.
			tToMakeInstSeparateVec.push_back(std::make_pair(bModelData, std::vector<RenderObjectInstancedMesh3D*>()));

			//3D Model �߽����� ��ȯ�ؾ� �Ѵ�.
			for (int i = 0; i < tVecVBSize; i++)
			{
				auto tInstancedMesh = bVecPtr.at(i)._instancedRenderObject.get();
				//�������� ��� ���.
				tToMakeInstSeparateVec.back().second.push_back(tInstancedMesh);
			}
		}

		auto t3DLoader = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetBasic3DLoader();
		for (int i = 0; i < tToMakeInstSeparateVec.size(); i++)
		{
			auto& tModel = tToMakeInstSeparateVec.at(i).first;

			BufferInstancedPairList* tBufferInstancedPairList = _currentListSet->_renderObject3DList->_instancedStaticList.at(tModel).get();
			auto& tVB = tBufferInstancedPairList->_vb;
			auto& tInstanceVector = tToMakeInstSeparateVec.at(i).second;
			//�̰� �����Ǵ� ��Ұ� �� ���̴�.

			//�ν��Ͻ��� ���� ObjID / MatID / Transform ���� �ε�.
			t3DLoader->LoadObjMatTRSBufferInstanced(tVB, tInstanceVector);
		}
	}


	void GraphicsSceneParser::CreateFrontFaceInstancedTRSBuffer()
	{
		//�Ųٷ� �ø��Ǿ�� �ϴ� ���.
		std::vector<std::pair<Asset3DModelData*, std::vector<RenderObjectInstancedMesh3D*>>> tToMakeInstSeparateVec;

		for (auto& [bModelData, bVecPair] : _currentListSet->_renderObject3DList->_instancedCulledOppositeStaticList)
		{
			auto& bVecPtr = bVecPair->_instancedStaticPairVec;

			//assert(bVecPtr != nullptr);
			//assert(!bVecPtr.empty());

			if (bVecPtr.empty())
			{
				continue;
			}

			unsigned int tVecVBSize = bVecPtr.size();

			//���� ��� �߰�.
			tToMakeInstSeparateVec.push_back(std::make_pair(bModelData, std::vector<RenderObjectInstancedMesh3D*>()));

			//3D Model �߽����� ��ȯ�ؾ� �Ѵ�.
			for (int i = 0; i < tVecVBSize; i++)
			{
				auto tInstancedMesh = bVecPtr.at(i)._instancedRenderObject.get();
				//�������� ��� ���.
				tToMakeInstSeparateVec.back().second.push_back(tInstancedMesh);
			}
		}

		auto t3DLoader = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetBasic3DLoader();
		for (int i = 0; i < tToMakeInstSeparateVec.size(); i++)
		{
			auto& tModel = tToMakeInstSeparateVec.at(i).first;
			auto& tMatchingIter = _currentListSet->_renderObject3DList->_instancedCulledOppositeStaticList.at(tModel);
			auto& tVB = tMatchingIter->_vb;
			auto& tInstanceVector = tToMakeInstSeparateVec.at(i).second;
			//�̰� �����Ǵ� ��Ұ� �� ���̴�.

			//�ν��Ͻ��� ���� ObjID / MatID / Transform ���� �ε�.
			t3DLoader->LoadObjMatTRSBufferInstanced(tVB, tInstanceVector);
		}
	}

	void GraphicsSceneParser::CreateClippedBackFaceInstancedTRSBuffer()
	{
		//�Ϲ����� Backface Culling ���.
		std::vector<std::pair<Asset3DModelData*, std::vector<RenderObjectInstancedMesh3D*>>> tToMakeInstSeparateVec;

		for (auto& [bModelData, bVecPair] : _currentListSet->_renderObject3DList->_instancedStaticAlphaClippedList)
		{
			//
			auto& bVecPtr = bVecPair->_instancedStaticPairVec;

			//assert(bVecPtr != nullptr);
			//assert(!bVecPtr.empty());
			if (bVecPtr.empty())
			{
				continue;
			}

			unsigned int tVecVBSize = bVecPtr.size();

			//���� ��� �߰�.
			tToMakeInstSeparateVec.push_back(std::make_pair(bModelData, std::vector<RenderObjectInstancedMesh3D*>()));

			//3D Model �߽����� ��ȯ�ؾ� �Ѵ�.
			for (int i = 0; i < tVecVBSize; i++)
			{
				auto tInstancedMesh = bVecPtr.at(i)._instancedRenderObject.get();
				//�������� ��� ���.
				tToMakeInstSeparateVec.back().second.push_back(tInstancedMesh);
			}
		}

		auto t3DLoader = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetBasic3DLoader();
		for (int i = 0; i < tToMakeInstSeparateVec.size(); i++)
		{
			auto& tModel = tToMakeInstSeparateVec.at(i).first;

			BufferInstancedPairList* tBufferInstancedPairList = _currentListSet->_renderObject3DList->_instancedStaticAlphaClippedList.at(tModel).get();
			auto& tVB = tBufferInstancedPairList->_vb;
			auto& tInstanceVector = tToMakeInstSeparateVec.at(i).second;
			//�̰� �����Ǵ� ��Ұ� �� ���̴�.

			//�ν��Ͻ��� ���� ObjID / MatID / Transform ���� �ε�.
			t3DLoader->LoadObjMatTRSBufferInstanced(tVB, tInstanceVector);
		}
	}

	void GraphicsSceneParser::CreateClippedFrontFaceInstancedTRSBuffer()
	{
		//�Ųٷ� �ø��Ǿ�� �ϴ� ���.
		std::vector<std::pair<Asset3DModelData*, std::vector<RenderObjectInstancedMesh3D*>>> tToMakeInstSeparateVec;

		for (auto& [bModelData, bVecPair] : _currentListSet->_renderObject3DList->_instancedCulledOppositeStaticAlphaClippedList)
		{
			auto& bVecPtr = bVecPair->_instancedStaticPairVec;

			//assert(bVecPtr != nullptr);
			//assert(!bVecPtr.empty());

			if (bVecPtr.empty())
			{
				continue;
			}

			unsigned int tVecVBSize = bVecPtr.size();

			//���� ��� �߰�.
			tToMakeInstSeparateVec.push_back(std::make_pair(bModelData, std::vector<RenderObjectInstancedMesh3D*>()));

			//3D Model �߽����� ��ȯ�ؾ� �Ѵ�.
			for (int i = 0; i < tVecVBSize; i++)
			{
				auto tInstancedMesh = bVecPtr.at(i)._instancedRenderObject.get();
				//�������� ��� ���.
				tToMakeInstSeparateVec.back().second.push_back(tInstancedMesh);
			}
		}

		auto t3DLoader = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetBasic3DLoader();
		for (int i = 0; i < tToMakeInstSeparateVec.size(); i++)
		{
			auto& tModel = tToMakeInstSeparateVec.at(i).first;
			auto& tMatchingIter = _currentListSet->_renderObject3DList->_instancedCulledOppositeStaticAlphaClippedList.at(tModel);
			auto& tVB = tMatchingIter->_vb;
			auto& tInstanceVector = tToMakeInstSeparateVec.at(i).second;
			//�̰� �����Ǵ� ��Ұ� �� ���̴�.

			//�ν��Ͻ��� ���� ObjID / MatID / Transform ���� �ε�.
			t3DLoader->LoadObjMatTRSBufferInstanced(tVB, tInstanceVector);
		}
	}

}