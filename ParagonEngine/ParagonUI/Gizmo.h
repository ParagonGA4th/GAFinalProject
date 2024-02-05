#pragma once

namespace Pg::Data { class Camera; class Transform; }
namespace Pg::UI::Helper
{
	class Gizmo
	{
	public:
		Gizmo();
		~Gizmo() {};

		void CreateFrame();
		void DrawGizmo();

		void SetCamera(Pg::Data::Camera* camera);
		void SetTransform(Pg::Data::Transform* trans);
		void SetWindowSize(float width, float height);

	private:
		Pg::Data::Camera* _camera;
		Pg::Data::Transform* _trans;

		float _displayWidth;
		float _displayHeight;
	};
}


