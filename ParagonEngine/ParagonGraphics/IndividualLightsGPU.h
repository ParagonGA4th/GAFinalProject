#pragma once
#include "../ParagonData/DirectionalLight.h"
#include "../ParagonData/SpotLight.h"
#include "../ParagonData/PointLight.h"

/// <summary>
/// ������Ʈ ��ü�� GPU�� ���� �� ���� ������, ���� ������ ��� ���ؼ� ������� POD ����ü��.
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

		float tTempPadding{ 0.f }; // ���� �ȵ�.
		
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
