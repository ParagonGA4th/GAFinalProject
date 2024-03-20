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

		//함수가 호출되었다는 것은 무조건 찾아야 한다는 것. 미리 아무것도 안 클릭되었을 경우를 걸러내기 때문.
		Pg::Data::GameObject* GetObjectWithObjID(unsigned int objID);
	private:
		void ClearMakeObjectLists();
		void PlacePathsFromName(const Pg::Data::Scene* const newScene);
		void CheckForPathNameErrors(const Pg::Data::Scene* const newScene);
		void ExtractMaterialPaths(const Pg::Data::Scene* const newScene);
		void SyncRenderObjects(const Pg::Data::Scene* const newScene);
		void SyncSceneInformation(const Pg::Data::Scene* const newScene);
		void BindAdequateFunctions(const Pg::Data::Scene* const newScene); //내부적으로 작동을 위한 bind될 함수들이 있으면, 이를 발동하기 위해.
		void CreateObjMatBuffersAll();

	private:
		//PrimitiveWireframeObject 하드코딩해서 완성.
		void SetupPrimitiveWireframeObjects();
		//일단은 Cubemap List는 하드코딩되어 있음.
		void PlaceCubemapList();
	private:
		//저장된 GameObject <=> RenderObject들.
		std::unique_ptr<RenderObject2DList> _renderObject2DList;
		std::unique_ptr<RenderObject3DList> _renderObject3DList;
		std::unique_ptr<RenderObjectCubemapList> _cubeMapList;
		//Axis/Grid
		std::unique_ptr<RenderObjectWireframeList> _primObjectList;
		//Scene 관련된 정보 Syncing을 위해 존재한다.
		std::unique_ptr<SceneInformationList> _sceneInfoList;
	private:
		//3DObject들의 ID를 기록해서 렌더링 엔진에 올린다.
		unsigned int _objectId3dCount = 1;
	};
}


