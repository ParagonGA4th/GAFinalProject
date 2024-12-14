#pragma once
#include "RenderObject2DList.h"
#include "RenderObject3DList.h"
#include "RenderObjectCubemapList.h"
#include "RenderObjectWireframeList.h"
#include "SceneInformationList.h"

#include "../ParagonData/GameObject.h"

#include <memory>
#include <vector>

/// <summary>
/// ���� ���� ������ ������ ���������ο� �°� �����ͼ� ������ �� �ְ� �Ѵ�!
/// </summary>

namespace Pg::Data
{
	class Scene;
	class GameObject;
}

namespace Pg::Graphics
{
	struct GraphicObjectListSet
	{
		//������� �� ���Ӱ� ��ü �Ҵ�.
		GraphicObjectListSet()
		{
			_renderObject2DList.reset(new RenderObject2DList());
			_renderObject3DList.reset(new RenderObject3DList());
			_cubeMapList.reset(new RenderObjectCubemapList());
			_primObjectList.reset(new RenderObjectWireframeList());
			_sceneInfoList.reset(new SceneInformationList());
		}

		std::unique_ptr<RenderObject2DList> _renderObject2DList;
		std::unique_ptr<RenderObject3DList> _renderObject3DList;
		std::unique_ptr<RenderObjectCubemapList> _cubeMapList;
		//Axis/Grid
		std::unique_ptr<RenderObjectWireframeList> _primObjectList;
		//Scene ���õ� ���� Syncing�� ���� �����Ѵ�.
		std::unique_ptr<SceneInformationList> _sceneInfoList;
	};
}

namespace Pg::Graphics
{
	class GraphicsSceneParser
	{
	public:
		GraphicsSceneParser();
		~GraphicsSceneParser();

		void Initialize();

		//������ ������Ʈ���� Sort�ؼ�, Pass�� ��� (N���� Material�� ���) �����ϰ� �Ѵ�.
		void ParseSceneData(const Pg::Data::Scene* const newScene);

		RenderObject2DList* GetRenderObject2DList();
		RenderObject3DList* GetRenderObject3DList();
		RenderObjectCubemapList* GetRenderObjectCubemapList();
		RenderObjectWireframeList* GetRenderObjectWireframeList();
		SceneInformationList* GetSceneInformationList();

		//[DEPRECATED] �Լ��� ȣ��Ǿ��ٴ� ���� ������ ã�ƾ� �Ѵٴ� ��. �̸� �ƹ��͵� �� Ŭ���Ǿ��� ��츦 �ɷ����� ����.
		[[deprecated("Not Using Anymore")]]
		Pg::Data::GameObject* GetObjectWithObjID(unsigned int objID);

		//���ӿ�����Ʈ �����͸� ������ ObjectID ���̵� ��ȯ�� �� �ִ�.
		unsigned int GetObjIDWithObject(const Pg::Data::GameObject* const obj);

		//�����Ϳ� ���� / ���� ������ ���� �߰� �߰��ϰ� �� ���.
		void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		
		//Scene�ý���ó�� �ϰ������� ������Ʈ.
		void HandleRenderObjectsRuntime();

	private:
		//ParseNewScene ����.
		void ResetParser();
		void PlacePathsFromName(const Pg::Data::Scene* const newScene);
		void CheckForPathNameErrors(const Pg::Data::Scene* const newScene);
		void RemapMaterialIdForUninitMaterials();
		void ExtractMaterialPaths(const Pg::Data::Scene* const newScene);
		void SyncRenderObjects(const Pg::Data::Scene* const newScene);
		void SyncSceneAllLights(const Pg::Data::Scene* const newScene);
		void CheckBindAdequateFunctions(); //���������� �۵��� ���� bind�� �Լ����� ������, �̸� �ߵ��ϱ� ����.
		void CheckCreateObjMatBuffersAll();

		void CreateAllInstancedRenderObjects(const Pg::Data::Scene* const newScene);

	private:
		//PrimitiveWireframeObject �ϵ��ڵ��ؼ� �ϼ�.
		void SetupPrimitiveWireframeObjects(const Pg::Data::Scene* const newScene);
		//�ϴ��� Cubemap List�� �ϵ��ڵ��Ǿ� ����.
		void PlaceCubemapList();

		//�߰��� �߰��� ���� ����ؼ�, 
		//�ܼ��� ���� �ִ� material Id�� ������ ���ο� ���� �־��� �� �߰��ϱ� ����.
		//���� �Ǿ ���������� �ʴ´�.
		void RemapAppendedMatID(); 

	private:
		//���������� Name -> Path Converting + Unreal ����.
		void PlaceCorrectPathSingleRenderer(Pg::Data::GameObject* tGameObject);
		//���������� PathName ���� üũ.
		void CheckPathNameErrorSingleRenderer(Pg::Data::GameObject* tGameObject);

		//Runtime�� �߰��� / ������ ������Ʈ : Material�� ������ �ʰ� �߰��� / ������ ��������� �Ѵ�.
		void RuntimeExtractMaterialPathsVector(const std::vector<Pg::Data::GameObject*>* runtimeObjList);
		
		//Runtime�� Lights GameObject Vector Sync! (���� �Ϸ�)
		void RuntimeAddLightsVector(const std::vector<Pg::Data::GameObject*>* objList);
		void RuntimeModifyLightsVector(const std::vector<Pg::Data::GameObject*>* objList);
		void RuntimeDeleteLightsVector(const std::vector<Pg::Data::GameObject*>* objList);
		void SortSceneInfoLightsVector();

		//���� SceneParser ���� ������Ʈ ������ (RenderObject)
		void AddSingleRenderObject(Pg::Data::GameObject* obj);
		void ModifySingleRenderObject(Pg::Data::GameObject* obj);
		void DeleteSingleRenderObject(Pg::Data::GameObject* obj);
	private:
		void CreateBackFaceInstancedTRSBuffer();
		void CreateFrontFaceInstancedTRSBuffer();
		void CreateClippedBackFaceInstancedTRSBuffer();
		void CreateClippedFrontFaceInstancedTRSBuffer();
	private:
		////����� GameObject <=> RenderObject��.
		//std::unique_ptr<RenderObject2DList> _renderObject2DList;
		//std::unique_ptr<RenderObject3DList> _renderObject3DList;
		//std::unique_ptr<RenderObjectCubemapList> _cubeMapList;
		////Axis/Grid
		//std::unique_ptr<RenderObjectWireframeList> _primObjectList;
		////Scene ���õ� ���� Syncing�� ���� �����Ѵ�.
		//std::unique_ptr<SceneInformationList> _sceneInfoList;
		
		//�� �� ���̺긦 ���ؼ�.
		//���� �� �˻��ϱ�. �̹� �ε��ߴ� ��Ҹ� �ٽ� �ε����� �ʰ�, 
		//����Ǿ� �ִ� ���� Ȱ��.
		//Scene Name / GraphicsListSet.
		std::unordered_map<std::string, std::unique_ptr<GraphicObjectListSet>> _graphicsObjectsListContainer;
		GraphicObjectListSet* _currentListSet{ nullptr };

	private:
		//3DObject���� ID�� ����ؼ� ������ ������ �ø���.
		UINT _objectId3dCount{ 1 };

	private:
		//��Ÿ�� ������ ���ؼ�.
		std::vector<Pg::Data::GameObject*> _runtimeAddedObjectList;
		std::vector<Pg::Data::GameObject*> _runtimeModifiedObjectList;
		std::vector<Pg::Data::GameObject*> _runtimeDeletedObjectList;
	
	};
}


