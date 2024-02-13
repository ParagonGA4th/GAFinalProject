#pragma once
#include "../ParagonMath/PgMath.h"

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
		void ConvertPTM(Pg::Math::PGFLOAT4X4& mt);
		void ConvertVTM(Pg::Math::PGFLOAT4X4& mt);
		void ConvertWTM(Pg::Math::PGFLOAT4X4& mt);

	private:
		Pg::Data::Camera* _camera;
		Pg::Data::Transform* _trans;

		float _cameraView[4][4];
		float _cameraProj[4][4];
		float _transMatrix[4][4];

		float _displayWidth;
		float _displayHeight;
	};
}


