#pragma once
#include <windows.h>
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Pg::UI::Manager
{
	class ImGuiManager
	{
	public:
		ImGuiManager();
		~ImGuiManager();

		void Initialize(void* hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		void CreateFrame();
		void Render();
		void Finalize();

		void ImguiHandler(MSG message);

		static void Begin(std::string windowName);
		static void End();
	};
}

