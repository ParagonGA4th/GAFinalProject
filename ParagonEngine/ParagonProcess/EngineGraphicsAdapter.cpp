#include "EngineGraphicsAdapter.h"
#include "AssetManager.h"
#include "ProcessMain.h"
#include "../ParagonData/RendererChangeList.h"
#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonGraphics/GraphicsDLLExporter.h"

#include "../ParagonGameEngine/EngineMain.h"
#include "../ParagonGraphics/GraphicsMain.h"

#include <windows.h>
#include <vector>
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGraphics.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGraphics.lib")
#endif // _DEBUG

namespace Pg::Core
{
	EngineGraphicsAdapter::EngineGraphicsAdapter(ProcessMain* core) : _processMain(core)
	{
		_engine = std::make_unique<Pg::Engine::EngineMain>(_processMain);
		_graphics = std::make_unique<Pg::Graphics::GraphicsMain>(_processMain);

		auto& tRendererChangeList = singleton<Pg::Data::RendererChangeList>();
		_rendererChangeList = &tRendererChangeList;
	}

	EngineGraphicsAdapter::~EngineGraphicsAdapter()
	{

	}

	void EngineGraphicsAdapter::InitializeEngine(void* hwnd, int screenWidth, int screenHeight)
	{
		_engine->Initialize(hwnd, screenWidth, screenHeight);
	}

	void EngineGraphicsAdapter::InitializeGraphics(void* hwnd, int screenWidth, int screenHeight)
	{
		_graphics->Initialize(static_cast<HWND>(hwnd), screenWidth, screenHeight);
	}

	void EngineGraphicsAdapter::UpdateEngine()
	{
		_engine->Update();
	}

	void EngineGraphicsAdapter::UpdateGraphics(const Pg::Data::Scene* const scene, Pg::Data::CameraData* cameraData)
	{
		_graphics->Update(scene, cameraData);
	}

	void EngineGraphicsAdapter::UpdateAssetManager(Manager::AssetManager* assetManager)
	{
		assetManager->Update(_engine.get(), _graphics.get());
	}

	void EngineGraphicsAdapter::BeginRender()
	{
		_graphics->BeginRender();
	}

	void EngineGraphicsAdapter::Render()
	{
		//워크스페이스 버전
		_graphics->Render(_engine->GetCurrentScene());
	}

	Pg::Data::GameObject* EngineGraphicsAdapter::GetPickedGameObjectWithRatios(float widthRatio, float heightRatio)
	{
		return _graphics->GetPickedGameObjectWithRatios(widthRatio, heightRatio);
	}

	void EngineGraphicsAdapter::FinalRender()
	{
		_graphics->FinalRender();
	}

	void EngineGraphicsAdapter::EndRender()
	{
		_graphics->EndRender();
	}

	void EngineGraphicsAdapter::Finalize()
	{
		_engine->Finalize();
		_graphics->Finalize();
		//..
	}

	void EngineGraphicsAdapter::OnWindowResized(int screenWidth, int screenHeight)
	{
		_graphics->OnWindowResized(screenWidth, screenHeight);
	}

	ID3D11Device* EngineGraphicsAdapter::GetGraphicsDevice()
	{
		return _graphics->GetDevice();
	}

	ID3D11DeviceContext* EngineGraphicsAdapter::GetGraphicsDeviceContext()
	{
		return _graphics->GetDeviceContext();
	}

	void EngineGraphicsAdapter::UpdateRendererChangeList()
	{
		if (_rendererChangeList->IfShouldUpdate())
		{
			//_graphics->SyncComponentToGraphics();
			_rendererChangeList->ClearRendererChangeLists();
		}
	}

	void* EngineGraphicsAdapter::GetEditorCameraViewSRV()
	{
		return _graphics->GetEditorCameraViewSRV();
	}

	void* EngineGraphicsAdapter::GetGameCameraViewSRV()
	{
		return _graphics->GetGameCameraViewSRV();
	}

	void EngineGraphicsAdapter::SetSceneList(std::vector<Pg::Data::Scene*> sceneList)
	{
		return _engine->SetSceneList(sceneList);
	}

	void EngineGraphicsAdapter::SetCurrentScene(Pg::Data::Scene* currentScene)
	{
		return _engine->SetCurrentScene(currentScene);
	}

	Pg::Data::Scene* EngineGraphicsAdapter::GetCurrentScene()
	{
		return _engine->GetCurrentScene();
	}

	Pg::Data::CameraData* EngineGraphicsAdapter::GetCameraData()
	{
		return _engine->GetCameraData();
	}

	void EngineGraphicsAdapter::SyncLoadGraphicsResources()
	{
		_graphics->SyncLoadGraphicsResources();
	}

	void EngineGraphicsAdapter::PassDebugRenderData()
	{
		const auto& tBoxColVec = _engine->GetBoxDebugData();
		_graphics->SetBoxDebugRenderData(tBoxColVec);

		const auto& tSphereColVec = _engine->GetSphereDebugData();
		_graphics->SetSphereDebugRenderData(tSphereColVec);

		const auto& tCapsuleColVec = _engine->GetCapsuleDebugData();
		_graphics->SetCapsuleDebugRenderData(tCapsuleColVec);

		const auto& tLineColVec = _engine->GetLineDebugData();
		_graphics->SetLineDebugRenderData(tLineColVec);

		const auto& tPlaneColVec = _engine->GetPlaneDebugData();
		_graphics->SetPlaneDebugRenderData(tPlaneColVec);

		const auto& tRayCastColVec = _engine->GetRayCastDebugData();
		_graphics->SetRayCastDebugRenderData(tRayCastColVec);

		const auto& tBox2dVec = _engine->GetBox2dDebugData();
		_graphics->SetBox2dDebugRenderData(tBox2dVec);
	}

	void EngineGraphicsAdapter::ClearDebugVectorData()
	{
		_engine->ClearDebugVectorData();
	}

	

}