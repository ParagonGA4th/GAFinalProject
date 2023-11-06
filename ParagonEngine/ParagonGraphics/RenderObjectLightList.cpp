#include "RenderObjectLightList.h"
#include "RenderObjectLight.h"
#include "../ParagonData/Light.h"
#include "LowDX11Storage.h"

#include "../ParagonData/Light.h"
#include "../ParagonData/DirectionalLight.h"
#include "../ParagonData/PointLight.h"
#include "../ParagonData/SpotLight.h"

Pg::Graphics::RenderObjectLightList::RenderObjectLightList()
	:_DXStorage(LowDX11Storage::GetInstance())
{

}

void Pg::Graphics::RenderObjectLightList::UpdateConstantBuffer()
{
	for (int i = 0; i < _constantBuffers.size(); ++i)
	{
		_constantBuffers[i]->UpdateAndBind(i);
	}
}

void Pg::Graphics::RenderObjectLightList::ParseLights(Pg::Data::Light* lightComponent)
{
	if (Pg::Data::DirectionalLight* directionalLight = dynamic_cast<Pg::Data::DirectionalLight*>(lightComponent))
	{
		Pg::Data::Structs::DirectionalLight* tLightData = new Pg::Data::Structs::DirectionalLight();

		tLightData->intensity = directionalLight->GetIntensity();
		tLightData->color = directionalLight->GetLightColor();
		tLightData->ambient = directionalLight->GetAmbient();
		tLightData->diffuse = directionalLight->GetDiffuse();
		tLightData->specular = directionalLight->GetSpecular();

		tLightData->direction = directionalLight->GetDirection();

		_directionalLight.emplace_back(*tLightData);
	}
	else if (Pg::Data::PointLight* pointLight = dynamic_cast<Pg::Data::PointLight*>(lightComponent))
	{
		Pg::Data::Structs::PointLight* tLightData = new Pg::Data::Structs::PointLight();

		tLightData->intensity = pointLight->GetIntensity();
		tLightData->color = pointLight->GetLightColor();
		tLightData->ambient = pointLight->GetAmbient();
		tLightData->diffuse = pointLight->GetDiffuse();
		tLightData->specular = pointLight->GetSpecular();

		tLightData->attenuation = pointLight->GetAttenuation();
		tLightData->range = pointLight->GetRange();

		_pointLight.emplace_back(*tLightData);
	}

	else if (Pg::Data::SpotLight* SpotLight = dynamic_cast<Pg::Data::SpotLight*>(lightComponent))
	{
		Pg::Data::Structs::SpotLight* tLightData = new Pg::Data::Structs::SpotLight();
		
		tLightData->intensity = SpotLight->GetIntensity();
		tLightData->color = SpotLight->GetLightColor();
		tLightData->ambient = SpotLight->GetAmbient();
		tLightData->diffuse = SpotLight->GetDiffuse();
		tLightData->specular = SpotLight->GetSpecular();

		tLightData->attenuation = SpotLight->GetAttenuation();
		tLightData->range = SpotLight->GetRange();

		_spotLight.emplace_back(*tLightData);
	}

}

void Pg::Graphics::RenderObjectLightList::BuildConstantBuffer()
{
	CreateConstantBuffer(_directionalLight.data(), _directionalLight.size());
	CreateConstantBuffer(_pointLight.data(), _pointLight.size());
	CreateConstantBuffer(_spotLight.data(), _spotLight.size());
}

void Pg::Graphics::RenderObjectLightList::ClearLightData()
{
	_directionalLight.clear();
	_pointLight.clear();
	_spotLight.clear();
}
