#pragma once
#include "IEngine.h"
#include "IGraphics.h"

#include <memory>
#include <vector>
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
		void InitializeEngine(int screenWidth, int screenHeight);

		//그래픽스 초기화.
		void InitializeGraphics(void* hwnd, int screenWidth, int screenHeight);

		//그래픽스 리소스, 애셋 매니저와 연동.
		void SyncLoadGraphicsResources();
		
		//Engine Update.
		void UpdateEngine();

		//Graphics Update.
		void UpdateGraphics(const Pg::Data::Scene* const scene, Pg::Data::CameraData* cameraData);

		//AssetManager Update.
		void UpdateAssetManager(Manager::AssetManager* assetManager);

		//렌더를 시작할 때.
		void BeginRender();

		//실제로 렌더를 시행할 때.
		void Render();

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
		void SetCurrentScene(Pg::Data::Scene* currentScene);
		Pg::Data::Scene* GetCurrentScene();

		Pg::Data::CameraData* GetCameraData();

		//그래픽스 엔진한테 렌더될 디버그 데이터를 전달한다.
		void PassDebugRenderData();

		//디버그 시스템 지오메트리 데이터 클리어.
		void ClearDebugVectorData();

	private:
		std::unique_ptr<IEngine> _engine;					//게임 엔진
		std::unique_ptr<IGraphics> _graphics;				//그래픽스 엔진

		ProcessMain* _processMain; //코어 메인 받아와서 저장.

		//렌더러 체인지 리스트 (연동 위해)
		Pg::Data::RendererChangeList* _rendererChangeList = nullptr;
	};
}


