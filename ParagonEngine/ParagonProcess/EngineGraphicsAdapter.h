#pragma once
#include "IEngine.h"
#include "IGraphics.h"
#include "../ParagonData/EditorMode.h"

#include <memory>
#include <vector>
#include <string>
#include <d3d11.h>
/// <summary>
/// Process에서 Engine과 Graphics를 조종할 수 있는 어댑터의 역할을 수행한다.
/// </summary>

namespace Pg::Core
{
	class ProcessMain;

	namespace Manager
	{
		class AssetManager;
	}
}

namespace Pg::Data
{
	class Scene;
	class GameObject;
	class RendererChangeList;
}

namespace Pg::Core
{
	class EngineGraphicsAdapter
	{
	public:
		EngineGraphicsAdapter(ProcessMain* core);
		~EngineGraphicsAdapter();

		//엔진 초기화.
		void InitializeEngine(void* hwnd, int screenWidth, int screenHeight, const std::string& resourceListPath);

		//그래픽스 초기화.
		void InitializeGraphics(void* hwnd, int screenWidth, int screenHeight, const std::string& resourceListPath);

		//그래픽스 리소스, 애셋 매니저와 연동.
		void SyncLoadGraphicsResources();
		
		//Engine Update.
		void UpdateEngine();

		//Graphics Update.
		void UpdateGraphics(const Pg::Data::Scene* const scene, float deltaTime, Pg::Data::CameraData* cameraData);

		//AssetManager Update.
		void UpdateAssetManager(Manager::AssetManager* assetManager);

		//렌더를 시작할 때.
		void BeginRender();

		//실제로 렌더를 시행할 때.
		void Render();
		
		////피킹된 오브젝트 받아야 함. (0-1 가로 비율 // 0-1 세로 비율)
		//Pg::Data::GameObject* GetPickedGameObjectWithRatios(float widthRatio, float heightRatio);
		
		//아웃라인되어야 할 게임오브젝트 선택.
		void SetOutlinedGameObject(Pg::Data::GameObject* obj);

		//Quad-Based 렌더링, Main RenderTarget으로.
		void FinalRender();
		
		//렌더를 끝낼 때.
		void EndRender();

		//Finalize.
		void Finalize();

		//윈도우의 사이즈가 변할 때.
		void OnWindowResized(int screenWidth, int screenHeight);

		//그래픽스 디바이스 반환.
		ID3D11Device* GetGraphicsDevice();

		//그래픽스 디바이스 컨텍스트 반환.
		ID3D11DeviceContext* GetGraphicsDeviceContext();

		//그래픽스 엔진이랑 Renderer 컴포넌트와 연동시킨다.
		void UpdateRendererChangeList();

		//Editor 연동 : Editor Camera 시점 SRV를 리턴.
		void* GetEditorCameraViewSRV();

		//Editor 연동 : Game Camera 시점 SRV를 리턴.
		void* GetGameCameraViewSRV();

		void SetSceneList(std::vector<Pg::Data::Scene*> sceneList);
		std::vector<Pg::Data::Scene*> GetSceneList();
		void SetCurrentScene(Pg::Data::Scene* currentScene);
		Pg::Data::Scene* GetCurrentScene();

		Pg::Data::CameraData* GetCameraData();

		//그래픽스 엔진한테 렌더될 디버그 데이터를 전달한다.
		void PassDebugRenderData();

		//디버그 시스템 지오메트리 데이터 클리어.
		void ClearDebugVectorData();

		//델타 타임 넘기기.
		float GetDeltaTime();

		//에디터가 명시적으로 EndRender 전에 사용가능.  에디터/게임 로직 위해서 런타임에 추가 / 수정 / 삭제 로직 열었다.
		void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);

		//에디터에 의해 직접 호출 X, 프로세스에 의해 일괄적으로 Add/Modify/Delete된 오브젝트 실제로 반영하는데 쓰일 것.
		void HandleRenderObjectsRuntime();

		//이미 전달된 

		// 현재 에디터의 상태를 받는 함수
		void SetEditorMode(Pg::Data::Enums::eEditorMode mode);

		//디폴트 리소스 로드된 후, 그래픽스에서 연동.
		void GraphicsConnectDefaultResources();

		//Paragon Project가 로딩되었을 때.
		void OnParagonProjectLoad(const std::vector<Pg::Data::Scene*>& sceneVec);

	private:
		std::unique_ptr<IEngine> _engine;					//게임 엔진
		std::unique_ptr<IGraphics> _graphics;				//그래픽스 엔진

		ProcessMain* _processMain; //코어 메인 받아와서 저장.

		//렌더러 체인지 리스트 (연동 위해)
		Pg::Data::RendererChangeList* _rendererChangeList = nullptr;
	};
}


