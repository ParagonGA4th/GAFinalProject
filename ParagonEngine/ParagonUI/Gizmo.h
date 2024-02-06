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
		// matType 0 = CameraView
		// matType 1 = CameraProj
		// matType 3 = Transform
		void ParagonMatrixToImGizmoMatrix(Pg::Math::PGFLOAT4X4 matrix, int matType);

	private:
		Pg::Data::Camera* _camera;
		Pg::Data::Transform* _trans;

		Pg::Math::PGFLOAT4X4 _imGizmoMatrix;

		float _cameraView[4][4];
		float _cameraProj[4][4];
		float _transMatrix[4][4];

		float _displayWidth;
		float _displayHeight;
	};
}


