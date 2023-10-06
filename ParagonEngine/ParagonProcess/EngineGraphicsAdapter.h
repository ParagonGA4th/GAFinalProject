#pragma once
#include "IEngine.h"
#include "IGraphics.h"

#include <memory>
#include <d3d11.h>
/// <summary>
/// Process에서 Engine과 Graphics를 조종할 수 있는 어댑터의 역할을 수행한다.
/// </summary>

namespace Pg::Core
{
	class CoreMain;

	namespace Manager
	{
		class AssetManager;
	}
}

namespace Pg::Data
{
	class Scene;
}

namespace Pg::Core
{
	class EngineGraphicsAdapter
	{
	public:
		EngineGraphicsAdapter(CoreMain* core);
		~EngineGraphicsAdapter();

		//엔진 초기화.
		void InitializeEngine(int screenWidth, int screenHeight);

		//그래픽스 초기화.
		void InitializeGraphics(void* hwnd, int screenWidth, int screenHeight);

		//Engine Update.
		void UpdateEngine();

		//Graphics Update.
		void UpdateGraphics(const Pg::Data::Scene* const scene, Pg::Data::CameraData cameraData, float deltaTime);

		//AssetManager Update.
		void UpdateAssetManager(Manager::AssetManager* assetManager);

		//렌더를 시작할 때.
		void BeginRender();

		//실제로 렌더를 시행할 때.
		void Render(Pg::Data::Scene* scene);

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

	private:
		std::unique_ptr<IEngine> _engine;					//게임 엔진
		std::unique_ptr<IGraphics> _graphics;				//그래픽스 엔진

		CoreMain* _coreMain; //코어 메인 받아와서 저장.
	};
}


