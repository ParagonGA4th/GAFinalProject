#pragma once
#include "../ParagonGraphics/GraphicsDLLExporter.h"
#include "../ParagonCore/IGraphics.h"
#include "../ParagonCore/Scene.h"
#include "../ParagonCore/CameraData.h"
#include "../ParagonCore/GameObject.h"

#include "TempCamera.h"
#include "TestCube.h"

#include <windows.h>
#include <memory>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

/// <summary>
/// 
/// 그래픽스 엔진의 메인 
/// 
/// </summary>

namespace Pg::Core
{
	class CoreMain;

	namespace Time
	{
		class TimeManager;
	}
}

namespace Pg::Graphics
{
	namespace Manager
	{
		class GraphicsResourceManager;
	}
}

namespace Pg::API
{
	class APIMain;

	namespace Input
	{
		class PgInput;
	}
}

namespace Pg::Graphics
{
	class LowDX11Logic;
	class LowDX11Storage;
	class ParagonRenderer;

	class GraphicsMain : public Pg::Core::IGraphics
	{
	public:
		PARAGON_GRAPHICS_DLL GraphicsMain(Pg::Core::CoreMain* core);
		virtual ~GraphicsMain();

	public:
		PARAGON_GRAPHICS_DLL virtual void Initialize(HWND hWnd, int screenWidth, int screenHeight) override;
		PARAGON_GRAPHICS_DLL virtual void Update(const Pg::Core::Scene* const scene, Pg::Core::CameraData cameraData) override;
		PARAGON_GRAPHICS_DLL virtual void BeginRender() override;
		PARAGON_GRAPHICS_DLL virtual void Render() override;
		PARAGON_GRAPHICS_DLL virtual void EndRender() override;
		PARAGON_GRAPHICS_DLL virtual void Finalize() override;

	public:
		PARAGON_GRAPHICS_DLL virtual ID3D11Device* GetDevice() override;
		PARAGON_GRAPHICS_DLL virtual ID3D11DeviceContext* GetDeviceContext() override;

		//AssetManager와의 교류를 위해, 그래픽스 리소스 매니저에 리소스를 로드하는 함수.
		PARAGON_GRAPHICS_DLL virtual void LoadResource(const std::string& filePath, Pg::Core::Enums::eAssetDefine define) override;

		// TODO: Load(Scene* )
		// 

	public:
		PARAGON_GRAPHICS_DLL virtual void OnWindowResized(int screenWidth, int screenHeight) override;

	private:
		HRESULT hr;
		Pg::Core::CoreMain* _coreMain;
		Pg::Graphics::Manager::GraphicsResourceManager* _graphicsResourceManager = nullptr;
	private:

		LowDX11Logic* _DXLogic;
		LowDX11Storage* _DXStorage;

	private:
		TempCamera* _camera;
		TestCube* _box;
		Pg::Core::GameObject* _tempObj;
	
	private:
		Pg::Core::Time::TimeManager* _timeManager;
		Pg::API::Input::PgInput* _inputManager;
		Pg::API::APIMain* _api;

	private:
		std::unique_ptr<ParagonRenderer> _renderer;
	};
}


