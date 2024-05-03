#pragma once
#include <windows.h>
#include <string>
#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Pg::Data { class Camera; class Transform; }
namespace Pg::UI::Helper { class Gizmo; }
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

		void Begin(std::string windowName, bool isMenu);
		void DockSpaceBegin(std::string dockName);
		void End(bool isDockspace);

		void BeginDisable(bool disable);
		void EndDisable();

		void SetGizmoCamera(Pg::Data::Camera* camera);
		void SetGizmoTransform(Pg::Data::Transform* trans);
		void DrawGizmo(std::string type);

		void AlignForWidth(int widgetCount, float widgetWidth);

		int IsFocus(std::string windowName);

		float GetMousePosX();
		float GetMousePosY();

		float GetWindowWidth();
		float GetWindowHeight();

		void OpenPopup(std::string popupName);

	private:
		//IMGUI 테마를 바꾼다.
		void ChangeStyle();
	private:
		std::unique_ptr<Pg::UI::Helper::Gizmo> _imGizmo;
		void* _pretendardFont{ nullptr };

		int _focusFlag = -1;
		bool _isHoverd;
	};
}

