#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "../ParagonMath/PgMath.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../ParagonData/Camera.h"
#include "../ParagonData/Transform.h"

#include <string>

namespace Pg::Data { class Camera; class Transform; }
namespace Pg::UI::Helper
{
	class Gizmo
	{
	public:
		Gizmo();
		~Gizmo() {};

		void CreateFrame();
		void DrawGizmo(std::string type);

		void SetCamera(Pg::Data::Camera* camera);
		void SetTransform(Pg::Data::Transform* trans);

	private:
		glm::mat4 ConvertVTM(Pg::Math::PGFLOAT4X4 mt);
		glm::mat4 ConvertWTM(Pg::Math::PGFLOAT4X4 mt);		
		
		void ConvertWTM(glm::mat4 mt);

	private:
		Pg::Data::Camera* _camera;
		Pg::Data::Transform* _trans;

		glm::mat4 _vtm;
		glm::mat4 _ptm;
		glm::mat4 _wtm;
	};
}


