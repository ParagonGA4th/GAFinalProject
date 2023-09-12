#pragma once

#include <d3d11.h>
#include <Windows.h>

/// <summary>
/// 에디터 인터페이스. 코어에 들어갈 예정
/// 2023.09.11
/// </summary>
namespace Pg::Core
{
	class IEditor abstract
	{
	public:
		virtual void  Initialize(HWND hWnd, ID3D11Device* , ID3D11DeviceContext*) abstract;

		virtual void Update() abstract;

		virtual void Finalize() abstract;
	};
}