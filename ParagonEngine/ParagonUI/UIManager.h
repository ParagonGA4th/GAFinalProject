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

namespace Pg::Data { class Camera; class Transform; }
namespace Pg::UI::Manager
{
	class ImGuiManager;
	class UIManager
	{
	public:
		 UIManager();
		 ~UIManager();

		 void Initialize(void* hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		 void Update();
		 void LastUpdate();
		 void Finalize();

		 void UIHandler(MSG message);

		 void SetAlignCenter(int widgetCount, float widgetWidth);

		 void SetCameraForGizmo(Pg::Data::Camera* camera);
		 void SetTransformForGizmo(Pg::Data::Transform* trans);
		 void SetGizmoType(void* gizmoType = nullptr);
		 void DrawGizmo();

		 void WindowBegin(std::string winName, bool isMenu = false);
		 void WindowDockSpaceBegin(std::string winName);
		 void WindowEnd(bool isDockspace = false);

		 void BeginDisable(bool disable);
		 void EndDisable();

		 float GetMousePosX();
		 float GetMousePosY();

		 float GetWindowWidth();
		 float GetWindowHeight();

		 float GetWindowTitleBarHeight();

		 float GetWindowPosX();
		 float GetWindowPosY();

		 void OpenPopup(std::string popupName);

	private:
		std::unique_ptr<Pg::UI::Manager::ImGuiManager> _imguiManager;
		std::string _gizmoType = "Translate";
	};
}
