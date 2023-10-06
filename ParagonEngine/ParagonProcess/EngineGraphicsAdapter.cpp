#include "EngineGraphicsAdapter.h"
#include "AssetManager.h"
#include "CoreMain.h"
#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonGraphics/GraphicsDLLExporter.h"

#include "../ParagonGameEngine/EngineMain.h"
#include "../ParagonGraphics/GraphicsMain.h"

#include <windows.h>

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
	EngineGraphicsAdapter::EngineGraphicsAdapter(CoreMain* core) : _coreMain(core)
	{
		_engine = std::make_unique<Pg::Engine::EngineMain>(_coreMain);
		_graphics = std::make_unique<Pg::Graphics::GraphicsMain>(_coreMain);
	}

	EngineGraphicsAdapter::~EngineGraphicsAdapter()
	{

	}

	void EngineGraphicsAdapter::InitializeEngine(int screenWidth, int screenHeight)
	{
		_engine->Initialize(screenWidth, screenHeight);
	}

	void EngineGraphicsAdapter::InitializeGraphics(void* hwnd, int screenWidth, int screenHeight)
	{
		_graphics->Initialize(static_cast<HWND>(hwnd), screenWidth, screenHeight);
	}

	void EngineGraphicsAdapter::UpdateEngine()
	{
		_engine->Update();
	}

	void EngineGraphicsAdapter::UpdateGraphics(const Pg::Data::Scene* const scene, Pg::Data::CameraData cameraData, float deltaTime)
	{
		_graphics->Update(scene, cameraData, deltaTime);
	}

	void EngineGraphicsAdapter::UpdateAssetManager(Manager::AssetManager* assetManager)
	{
		assetManager->Update(_engine.get(), _graphics.get());
	}

	void EngineGraphicsAdapter::BeginRender()
	{
		_graphics->BeginRender();
	}

	void EngineGraphicsAdapter::Render(Pg::Data::Scene* scene)
	{
		_graphics->Render(scene);
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



}