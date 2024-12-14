#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include <string>

// UI�� ����� ������ �� �־�� �Ѵ�
// ������� UI�� �ٸ� UI�� �߰� �� �� �־�� �Ѵ�


// imgui �ʱ�ȭ
// UI ��Ÿ��(��Ʈ, ����(��ũ���, ����Ʈ��� ��))
// Create
// Delete -> panel ��ü�� ���� ������, ���� �ִ� Widget�� ���� ������ ����
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
