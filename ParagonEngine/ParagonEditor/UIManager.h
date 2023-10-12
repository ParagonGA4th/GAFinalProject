#pragma once
#include <windows.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Pg::Editor::Manager
{
	class UIManager
	{
	public:
		UIManager();
		~UIManager();
		
		void Initialize(void* hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		void Update();
		void Render();
		void Finalize();

		bool Handler(HWND hWnd, UINT message, WPARAM WParam, LPARAM LParam);

	private:
		bool _testShow;
	};
}

