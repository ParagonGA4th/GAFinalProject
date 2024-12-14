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
/// 따로 씬의 내용을 렌더링 파이프라인에 맞게 가져와서 실행할 수 있게 한다!
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
		//만들어질 때 새롭게 객체 할당.
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
		//Scene 관련된 정보 Syncing을 위해 존재한다.
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

		//내부적 오브젝트별로 Sort해서, Pass별 출력 (N개의 Material에 비례) 가능하게 한다.
		void ParseSceneData(const Pg::Data::Scene* const newScene);

		RenderObject2DList* GetRenderObject2DList();
		RenderObject3DList* GetRenderObject3DList();
		RenderObjectCubemapList* GetRenderObjectCubemapList();
		RenderObjectWireframeList* GetRenderObjectWireframeList();
		SceneInformationList* GetSceneInformationList();

		//[DEPRECATED] 함수가 호출되었다는 것은 무조건 찾아야 한다는 것. 미리 아무것도 안 클릭되었을 경우를 걸러내기 때문.
		[[deprecated("Not Using Anymore")]]
		Pg::Data::GameObject* GetObjectWithObjID(unsigned int objID);

		//게임오브젝트 포인터를 가지고 ObjectID 아이디를 반환할 수 있다.
		unsigned int GetObjIDWithObject(const Pg::Data::GameObject* const obj);

		//에디터에 의해 / 게임 로직에 의해 중간 추가하게 될 경우.
		void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		
		//Scene시스템처럼 일괄적으로 업데이트.
		void HandleRenderObjectsRuntime();

	private:
		//ParseNewScene 내부.
		void ResetParser();
		void PlacePathsFromName(const Pg::Data::Scene* const newScene);
		void CheckForPathNameErrors(const Pg::Data::Scene* const newScene);
		void RemapMaterialIdForUninitMaterials();
		void ExtractMaterialPaths(const Pg::Data::Scene* const newScene);
		void SyncRenderObjects(const Pg::Data::Scene* const newScene);
		void SyncSceneAllLights(const Pg::Data::Scene* const newScene);
		void CheckBindAdequateFunctions(); //내부적으로 작동을 위한 bind될 함수들이 있으면, 이를 발동하기 위해.
		void CheckCreateObjMatBuffersAll();

		void CreateAllInstancedRenderObjects(const Pg::Data::Scene* const newScene);

	private:
		//PrimitiveWireframeObject 하드코딩해서 완성.
		void SetupPrimitiveWireframeObjects(const Pg::Data::Scene* const newScene);
		//일단은 Cubemap List는 하드코딩되어 있음.
		void PlaceCubemapList();

		//중간에 추가될 때를 대비해서, 
		//단순히 현재 있는 material Id와 별도로 새로운 값이 있었을 시 추가하기 위해.
		//제거 되어도 삭제되지는 않는다.
		void RemapAppendedMatID(); 

	private:
		//개별적으로 Name -> Path Converting + Unreal 연동.
		void PlaceCorrectPathSingleRenderer(Pg::Data::GameObject* tGameObject);
		//개별적으로 PathName 에러 체크.
		void CheckPathNameErrorSingleRenderer(Pg::Data::GameObject* tGameObject);

		//Runtime에 추가된 / 수정된 오브젝트 : Material을 뻑나지 않게 추가로 / 원래도 연동해줘야 한다.
		void RuntimeExtractMaterialPathsVector(const std::vector<Pg::Data::GameObject*>* runtimeObjList);
		
		//Runtime에 Lights GameObject Vector Sync! (구현 완료)
		void RuntimeAddLightsVector(const std::vector<Pg::Data::GameObject*>* objList);
		void RuntimeModifyLightsVector(const std::vector<Pg::Data::GameObject*>* objList);
		void RuntimeDeleteLightsVector(const std::vector<Pg::Data::GameObject*>* objList);
		void SortSceneInfoLightsVector();

		//실제 SceneParser 개별 오브젝트 구현부 (RenderObject)
		void AddSingleRenderObject(Pg::Data::GameObject* obj);
		void ModifySingleRenderObject(Pg::Data::GameObject* obj);
		void DeleteSingleRenderObject(Pg::Data::GameObject* obj);
	private:
		void CreateBackFaceInstancedTRSBuffer();
		void CreateFrontFaceInstancedTRSBuffer();
		void CreateClippedBackFaceInstancedTRSBuffer();
		void CreateClippedFrontFaceInstancedTRSBuffer();
	private:
		////저장된 GameObject <=> RenderObject들.
		//std::unique_ptr<RenderObject2DList> _renderObject2DList;
		//std::unique_ptr<RenderObject3DList> _renderObject3DList;
		//std::unique_ptr<RenderObjectCubemapList> _cubeMapList;
		////Axis/Grid
		//std::unique_ptr<RenderObjectWireframeList> _primObjectList;
		////Scene 관련된 정보 Syncing을 위해 존재한다.
		//std::unique_ptr<SceneInformationList> _sceneInfoList;
		
		//씬 별 세이브를 위해서.
		//들어온 거 검사하기. 이미 로드했던 요소면 다시 로드하지 않고, 
		//저장되어 있는 정보 활용.
		//Scene Name / GraphicsListSet.
		std::unordered_map<std::string, std::unique_ptr<GraphicObjectListSet>> _graphicsObjectsListContainer;
		GraphicObjectListSet* _currentListSet{ nullptr };

	private:
		//3DObject들의 ID를 기록해서 렌더링 엔진에 올린다.
		UINT _objectId3dCount{ 1 };

	private:
		//런타임 수정을 위해서.
		std::vector<Pg::Data::GameObject*> _runtimeAddedObjectList;
		std::vector<Pg::Data::GameObject*> _runtimeModifiedObjectList;
		std::vector<Pg::Data::GameObject*> _runtimeDeletedObjectList;
	
	};
}


