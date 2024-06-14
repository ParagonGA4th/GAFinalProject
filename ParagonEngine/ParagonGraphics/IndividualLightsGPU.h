#pragma once
#include "../ParagonData/DirectionalLight.h"
#include "../ParagonData/SpotLight.h"
#include "../ParagonData/PointLight.h"

/// <summary>
/// 컴포넌트 자체를 GPU에 담을 수 없기 때문에, 동일 내용을 담기 위해서 만들어진 POD 구조체들.
/// </summary>

namespace Pg::Graphics
{
	struct DirLightGPU
	{
		DirLightGPU() {}
		DirLightGPU(Pg::Data::DirectionalLight* light) :
			_color(light->_color), _radiance(light->_radiance),
			_direction(Pg::Math::GetForwardVectorFromQuat(light->_object->_transform._rotation)) {}

		Pg::Math::PGFLOAT3 _color;
		float _radiance;
		Pg::Math::PGFLOAT3 _direction;

		float tTempPadding{ 0.f }; // 쓰면 안됨.
		
	};

	struct SpotLightGPU
	{
		SpotLightGPU() {}
		SpotLightGPU(Pg::Data::SpotLight* light) :
			_color(light->_color), _radiance(light->_radiance),
			_direction(Pg::Math::GetForwardVectorFromQuat(light->_object->_transform._rotation)), _range(light->_range), _attenuation(light->_attenuation) {}

		Pg::Math::PGFLOAT3 _color;
		float _radiance;
		Pg::Math::PGFLOAT3 _direction;
		float _range;
		Pg::Math::PGFLOAT3 _attenuation;

		float tTempPadding{ 0.f };
	};

	struct PointLightGPU
	{
		PointLightGPU() {}
		PointLightGPU(Pg::Data::PointLight* light) : 
			_color(light->_color), _radiance(light->_radiance),
			_range(light->_range), _attenuation(light->_attenuation)
		{}

		Pg::Math::PGFLOAT3 _color;
		float _radiance;
		float _range;
		Pg::Math::PGFLOAT3 _attenuation;

	};
}
