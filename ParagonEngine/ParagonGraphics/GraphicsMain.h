#pragma once
#include "../ParagonGraphics/GraphicsDLLExporter.h"
#include "../ParagonProcess/IGraphics.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonMath/PgMath.h"

#include "TempCamera.h"
#include "TestCube.h"
#include "MultimaterialMesh.h"

#include <windows.h>
#include <memory>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonData.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonData.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonMath.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonMath.lib")
#endif // _DEBUG

/// <summary>
/// 
/// 그래픽스 엔진의 메인 
/// 
/// </summary>

class ID3D11ShaderResourceView;

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	}
}

namespace Pg::Core
{
	class ProcessMain;
	class Scene;

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

namespace Pg::Graphics
{
	class LowDX11Logic;
	class LowDX11Storage;
	class ParagonRenderer;

	class GraphicsMain : public Pg::Core::IGraphics
	{
	public:
		PARAGON_GRAPHICS_DLL GraphicsMain(Pg::Core::ProcessMain* core);
		virtual ~GraphicsMain();

	public:
		PARAGON_GRAPHICS_DLL virtual void Initialize(HWND hWnd, int screenWidth, int screenHeight) override;
		PARAGON_GRAPHICS_DLL virtual void SyncLoadGraphicsResources() override;

		PARAGON_GRAPHICS_DLL virtual void Update(const Pg::Data::Scene* const scene, Pg::Data::CameraData* cameraData, float deltaTime) override;
		PARAGON_GRAPHICS_DLL virtual void BeginRender() override;
		PARAGON_GRAPHICS_DLL virtual void Render(Pg::Data::Scene* scene) override;
		PARAGON_GRAPHICS_DLL virtual void EndRender() override;
		PARAGON_GRAPHICS_DLL virtual void Finalize() override;

	public:
		PARAGON_GRAPHICS_DLL virtual ID3D11Device* GetDevice() override;
		PARAGON_GRAPHICS_DLL virtual ID3D11DeviceContext* GetDeviceContext() override;

		PARAGON_GRAPHICS_DLL virtual void SyncComponentToGraphics() override;

		//AssetManager와의 교류를 위해, 그래픽스 리소스 매니저에 리소스를 로드하는 함수.
		PARAGON_GRAPHICS_DLL virtual void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define) override;

		//AssetManager와의 교류를 위해, 그래픽스 리소스 매니저에 리소스를 언로드하는 함수.
		PARAGON_GRAPHICS_DLL virtual void UnloadResource(const std::string& filePath) override;

		// TODO: Load(Scene* )
		// 

		//그래픽스 한정, 특정 리소스를 로드했을 시 내부 파생 2차 리소스 (Ex. Embedded Textures) 등등이 존재 가능.
		//이를 메인 목록과 호환하기 위해서 쓰이는 함수.
		PARAGON_GRAPHICS_DLL virtual std::map<std::string, Pg::Data::Enums::eAssetDefine>* SendAddedSecondaryResources() override;

		PARAGON_GRAPHICS_DLL virtual void ClearSecondaryResourcesList() override;

		//Editor 연동 : Editor Camera 시점 SRV를 리턴.
		PARAGON_GRAPHICS_DLL virtual ID3D11ShaderResourceView* GetEditorCameraViewSRV() override;
			
		//Editor 연동 : Game Camera 시점 SRV를 리턴.
		PARAGON_GRAPHICS_DLL virtual ID3D11ShaderResourceView* GetGameCameraViewSRV() override;

	public:
		PARAGON_GRAPHICS_DLL virtual void OnWindowResized(int screenWidth, int screenHeight) override;
	private:
		//불완전하게 전달된 CamData에 투영 행렬을 넣는다.
		void FillCamDataProjection(Pg::Data::CameraData* camData);

		//기능 중심 구현 요구 사항 옮겨놓았다. (Ex. Cubemap / Box.. etc..)
		//void BasicRendersInitialize();
		//void BasicRendersConstantBufferLoad();
		//void BasicRendersDraw();

		//원래는 Engine에서 필요한 리소스들을 씬의 상태/양상에 따라 로드해왔어야 하지만, 지금은 그럴 수 없으니 임시.
		void TempResourceMeshLoad();
	private:
		HRESULT hr;
		Pg::Core::ProcessMain* _coreMain;
		Pg::Graphics::Manager::GraphicsResourceManager* _graphicsResourceManager = nullptr;
	private:
		LowDX11Logic* _DXLogic;
		LowDX11Storage* _DXStorage;

	private:
		
		//TempCamera* _camera;
		//TestCube* _box;
		Pg::Data::GameObject* _tempObj;
		MultimaterialMesh* _tempMultiMesh;
	
	private:
		Pg::API::Input::PgInput* _input;
		Pg::Data::CameraData* _camData;

	private:
		// Editor 연동 & 나중에 이 SRV들이 최종 렌더되는 Quad의 SRV여야 한다.
		ID3D11ShaderResourceView* _editorCameraSRV = nullptr;
		ID3D11ShaderResourceView* _gameCameraSRV = nullptr;

	private:
		std::unique_ptr<ParagonRenderer> _renderer;
		Pg::Data::Scene* _currentScene = nullptr;
	};
}


