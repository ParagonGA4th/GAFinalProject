#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include <string>

// UI를 만들고 삭제할 수 있어야 한다
// 만들어진 UI에 다른 UI를 추가 할 수 있어야 한다


// imgui 초기화
// UI 스타일(폰트, 색상(다크모드, 라이트모드 등))
// Create
// Delete -> panel 자체를 지울 것인지, 속해 있는 Widget을 지울 것인지 선택
// Add

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Pg::UI::Manager
{
	class ImGuiManager;
	class UIManager
	{
	public:
		 UIManager();
		 ~UIManager();

		 void Initialize(void* hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		 void Update(void* Texture);
		 void LastUpdate();
		 void Finalize();

		//void SetFont();

		 void UIHandler(MSG message);

		 void WindowBegin(std::string winName);
		 void WindowEnd();

	private:
		std::unique_ptr<Pg::UI::Manager::ImGuiManager> _imguiManager;
	};
}
