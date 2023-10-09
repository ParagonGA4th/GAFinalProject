#pragma once

#include "../ParagonData/Scene.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/AssetDefines.h"
#include <string>
#include <windows.h>

/// <summary>
/// 변지상의 엔진 인터페이스. 코어에 들어갈 예정
/// 2023.09.11
/// </summary>
/// 
struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Pg::Data
{
	class Scene;
}

namespace Pg::Core
{
	class IGraphics abstract
	{
	public:
		virtual void Initialize(HWND hWnd, int screenWidth, int screenHeight) abstract;

		virtual void Update(const Pg::Data::Scene* const scene, Pg::Data::CameraData cameraData, float deltaTime) abstract;
		virtual void BeginRender() abstract;
		virtual void Render(Pg::Data::Scene* scene) abstract;
		virtual void EndRender() abstract;

		virtual void Finalize() abstract;

		virtual ID3D11Device* GetDevice() abstract;
		virtual ID3D11DeviceContext* GetDeviceContext() abstract;

		virtual void SyncComponentToGraphics() abstract;

		virtual void OnWindowResized(int screenWidth, int screenHeight) abstract;

		//AssetManager와의 교류를 위해, 그래픽스 리소스 매니저에 리소스를 로드하는 함수.
		virtual void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define) abstract;

		//AssetManager와의 교류를 위해, 그래픽스 리소스 매니저에 리소스를 언로드하는 함수.
		virtual void UnloadResource(const std::string& filePath) abstract;
	};
}