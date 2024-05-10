#pragma once
#include "D3DCarrier.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/DebugData.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/EditorMode.h"

//SkinningMk.2
//#include "MultimaterialMesh.h"

#include <memory>
#include <vector>

/// <summary>
/// Paragon Renderer
/// 
/// 2023. 09. 12.
/// </summary>

namespace Pg::Data
{
	class Scene;
	class GameObject;
}

namespace Pg::Graphics
{
	class LowDX11Logic;
	class LowDX11Storage;
	class DeferredRenderer;
	class CubemapRenderer;
	class Forward2DRenderer;
	class Forward3DRenderer;
	class DebugRenderer;
	class PPFinalRenderer;
	class GraphicsSceneParser;

	class SceneInformationList;
}

namespace Pg::Graphics
{
	class ParagonRenderer
	{
	public:
		ParagonRenderer();
		~ParagonRenderer();

		//Render 관련 Component 데이터를 다시 로드한다. -> 이 함수에 원래는 발동되어야 하는데..
		virtual void SyncComponentToGraphics(const Pg::Data::Scene* const newScene);
		
		//Scene이 바뀌었을 때 / 게임 엔진이 처음 시작되었을 때 호출되어야 한다.
		//메모리 추가 할당을 막기 위해, Scene당 렌더오브젝트 생성 로직 중복을 막아야 한다!
		
	public:
		void Initialize(const Pg::Data::Enums::eEditorMode* const editorMode);

		//DebugRenderer로 Debug Geometry를 넘겼다.
		void PassBoxGeometryData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec);
		void PassLineGeometryData(const std::vector<Pg::Data::LineInfo>& const lineColVec);
		void PassCapsuleGeometryData(const std::vector<Pg::Data::CapsuleInfo*>& const capsuleColVec);
		void PassSphereGeometryData(const std::vector<Pg::Data::SphereInfo*>& const sphereColVec);
		void PassPlaneGeometryData(const std::vector<Pg::Data::PlaneInfo*>& const planeColVec);
		void PassRayCastGeometryData(const std::vector<Pg::Data::RayCastInfo>& const rayCastColVec);
		void PassBox2dGeometryData(const std::vector<Pg::Data::Box2DInfo>& const box2dColVec);
		void PassNavMeshGeometryData(const std::vector<Pg::Data::NavMeshInfo*>& const navMeshVec);

		//Picking된 게임오브젝트를 보낸다.
		//[[deprecated("Not Using Picking Anymore")]]
		//Pg::Data::GameObject* GetPickedID_SetOutlineMode(int selectedWidthPixel, int selectedHeightPixel);
		
		//자체적으로 렌더러가 없는 게임오브젝트라면 아웃라인 패스를 실행하지 않는다.
		void SetOutlinedGameObject(Pg::Data::GameObject* outlinedObj);

		void BeginRender();
		void Render(Pg::Data::CameraData* camData);			// 이미 컴포넌트 단계에서 RenderObject들과 연동되기에, 오브젝트 자체를 받을 필요가 없음.
		void DebugRender(Pg::Data::CameraData* camData);	// 별도로 Debug Render를 한다.
		void UiRender(Pg::Data::CameraData* camData);		// 2D UI를 렌더링한다.
		void PostProcessingRender(Pg::Data::CameraData* camData); //PostProcessing을 렌더링한다.
		void FinalRender(Pg::Data::CameraData* camData);	// MainRenderTarget으로 보내주는 역할을 한다.
		void EndRender();

		//마지막 SRV Quad를 보낸다.
		ID3D11ShaderResourceView* GetFinalQuadSRV();

		//DeltaTime을 넘겨받는다.
		void SetDeltaTime(float dt);

		//에디터 연동을 위해.
		void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);

		//실제로 연동해서 처리 (기존에 Add/Modify/Delete되었던 오브젝트 변화 실제 반영)
		void HandleRenderObjectsRuntime();

	private:
		//씬 데이터 받아들이기. (렌더에 적합한 형태로)
		void ParseSceneData(const Pg::Data::Scene* const newScene);

	private:
		LowDX11Storage* _DXStorage = nullptr;
		LowDX11Logic* _DXLogic = nullptr;

		std::unique_ptr<GraphicsSceneParser> _sceneParser;

		std::unique_ptr<DeferredRenderer> _deferredRenderer;
		std::unique_ptr<CubemapRenderer> _cubemapRenderer;
		std::unique_ptr<Forward2DRenderer> _forward2dRenderer;
		std::unique_ptr<Forward3DRenderer> _forward3dRenderer;
		std::unique_ptr<DebugRenderer> _debugRenderer;
		std::unique_ptr<PPFinalRenderer> _finalRenderer;

	private:
		//저장용 X, 객체를 임시로 보관할 수 있게 매개변수용 자료 세팅.
		//스코프는 ParagonRenderer가 관리하고 있다.
		std::unique_ptr<D3DCarrier> _gCarrier;
	private:
		//SkinningMk.2 한정.
		//MultimaterialMesh* _tempMultiMesh;

		

	};
}

	