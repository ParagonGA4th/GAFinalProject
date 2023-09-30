#pragma once

#include "Scene.h"
#include "../ParagonCore/CameraData.h"
#include "AssetDefines.h"
#include <string>
#include <windows.h>

/// <summary>
/// 변지상의 엔진 인터페이스. 코어에 들어갈 예정
/// 2023.09.11
/// </summary>
/// 
struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Pg::Core
{
	class IGraphics abstract
	{
	public:
		virtual void Initialize(HWND hWnd, int screenWidth, int screenHeight) abstract;

		virtual void Update(const Pg::Core::Scene* const scene, Pg::Core::CameraData cameraData) abstract;
		virtual void BeginRender() abstract;
		virtual void Render() abstract;
		virtual void EndRender() abstract;

		virtual void Finalize() abstract;

		virtual ID3D11Device* GetDevice() abstract;
		virtual ID3D11DeviceContext* GetDeviceContext() abstract;

		virtual void OnWindowResized(int screenWidth, int screenHeight) abstract;

		//AssetManager와의 교류를 위해, 그래픽스 리소스 매니저에 리소스를 로드하는 함수.
		virtual void LoadResource(const std::string& filePath, Pg::Core::Enums::eAssetDefine define) abstract;
	};
}