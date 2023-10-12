#include "Light.h"

namespace Pg::Data
{
	Light::Light(GameObject* obj)
		: Component(obj),
		_ambient(1.f, 1.f, 1.f, 1.f),
		_specular(1.f, 1.f, 1.f, 1.f),
		_diffuse(1.f, 1.f, 1.f, 1.f)
	{

	}

	void Light::SetAmbient(PGFLOAT4 rad)
	{
		_ambient = rad;
	}

	PGFLOAT4 Light::GetAmbient()
	{
		return _ambient;
	}

	void Light::SetSpecular(PGFLOAT4 rad)
	{
		_specular = rad;
	}

	PGFLOAT4 Light::GetSpecular()
	{
		return _specular;
	}

	void Light::SetDiffuse(PGFLOAT4 rad)
	{
		_diffuse = rad;
	}

	PGFLOAT4 Light::GetDiffuse()
	{
		return _diffuse;
	}

	void Light::SetIntensity(float d)
	{
		_intensity = d;
	}

	float Light::GetIntensity()
	{
		return _intensity;
	}

	void Light::SetLightColor(PGFLOAT4 color)
	{
		_color = color;
	}

	void Light::SetLightColor(float r, float g, float b, float a)
	{
		PGFLOAT4 color = { r, g, b, a };
		SetLightColor(color);
	}

	Pg::Math::PGFLOAT4 Light::GetLightColor()
	{
		return _color;
	}

}