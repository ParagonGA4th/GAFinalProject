#pragma once
#include "../ParagonGraphics/GraphicsDLLExporter.h"
#include "../ParagonCore/IGraphics.h"

#include <windows.h>
//#include "DX11Headers.h"

/// <summary>
/// 
/// 그래픽스 엔진의 메인 
/// 
/// 23. 09. 08. 고태욱
/// </summary>
namespace Pg::Graphics
{
	class LowDX11Logic;
	class LowDX11Storage;

	class GraphicsMain : public Pg::Core::IGraphics
	{
	public:
		PARAGON_GRAPHICS_DLL GraphicsMain();

	public:
		PARAGON_GRAPHICS_DLL virtual void Initialize(HWND hWnd, int screenWidth, int screenHeight) override;
		PARAGON_GRAPHICS_DLL virtual void Update() override;
		PARAGON_GRAPHICS_DLL virtual void BeginRender() override;
		PARAGON_GRAPHICS_DLL virtual void Render() override;
		PARAGON_GRAPHICS_DLL virtual void EndRender() override;
		PARAGON_GRAPHICS_DLL virtual void Finalize() override;

	private:
		HRESULT hr;

	private:
		LowDX11Logic* _DXLogic;
		LowDX11Storage* _DXStorage;

	private:
		int _screenWidth;
		int _screenHeight;

	};
}


