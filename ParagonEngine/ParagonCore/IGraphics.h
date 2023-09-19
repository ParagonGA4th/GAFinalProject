#pragma once

#include "Scene.h"
#include "CameraData.h"

#include <windows.h>

/// <summary>
/// 변지상의 엔진 인터페이스. 코어에 들어갈 예정
/// 2023.09.11
/// </summary>
/// 
class ID3D11Device;
class ID3D11DeviceContext;
namespace Pg::Core
{


	class IGraphics abstract
	{
	public:
		virtual void Initialize(HWND hWnd, int screenWidth, int screenHeight) abstract;

		virtual void Update(const Scene* const scene, Pg::Core::CameraData cameraData) abstract;
		virtual void BeginRender() abstract;
		virtual void Render() abstract;
		virtual void EndRender() abstract;

		virtual void Finalize() abstract;

		virtual ID3D11Device* GetDevice() abstract;
		virtual ID3D11DeviceContext* GetDeviceContext() abstract;

		virtual void OnWindowResized(int screenWidth, int screenHeight) abstract;
	};
}