#include "RenderObjectLightList.h"
#include "RenderObjectLight.h"
#include "../ParagonData/Light.h"
#include "LowDX11Storage.h"

#include "../ParagonData/Light.h"
#include "../ParagonData/DirectionalLight.h"
#include "../ParagonData/PointLight.h"
#include "../ParagonData/SpotLight.h"

#include "../ParagonData/Transform.h"

#include "../ParagonAPI/PgInput.h"

Pg::Graphics::RenderObjectLightList::RenderObjectLightList()
	:_DXStorage(LowDX11Storage::GetInstance()), _lightingData()
{
	auto& tInputSystem = singleton<Pg::API::Input::PgInput>();
	_input = &tInputSystem;

	_lightingData.bufferToRender = 7;
}

void Pg::Graphics::RenderObjectLightList::UpdateConstantBuffer()
{
	for (int i = 0; i < _constantBuffers.size(); ++i)
	{
		_constantBuffers[i]->UpdateAndBind(i);
	}
}

void Pg::Graphics::RenderObjectLightList::ParseLights(Pg::Data::Transform* transform, Pg::Data::Light* lightComponent)
{
	// 광원의 종류에 따라 다른 리스트에 저장한다
	if (Pg::Data::DirectionalLight* directionalLight = dynamic_cast<Pg::Data::DirectionalLight*>(lightComponent))
	{
		Pg::Data::Structs::DirectionalLight* tLightData = new Pg::Data::Structs::DirectionalLight();

		tLightData->position = Pg::Math::PGFLOAT4(transform->GetPosition(), 0.0f);

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

		tLightData->position = Pg::Math::PGFLOAT4(transform->GetPosition(), 0.0f);

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
		
		tLightData->position = Pg::Math::PGFLOAT4(transform->GetPosition(), 0.0f);

		tLightData->intensity = SpotLight->GetIntensity();
		tLightData->color = SpotLight->GetLightColor();
		tLightData->ambient = SpotLight->GetAmbient();
		tLightData->diffuse = SpotLight->GetDiffuse();
		tLightData->specular = SpotLight->GetSpecular();

		tLightData->direction = SpotLight->GetDirection();

		tLightData->attenuation = SpotLight->GetAttenuation();
		tLightData->range = SpotLight->GetRange();

		_spotLight.emplace_back(*tLightData);
	}

}

void Pg::Graphics::RenderObjectLightList::BuildConstantBuffer()
{
	// 각각의 리스트마다 상수버퍼를 만들어준다
	// TODO: 광원 종류에 상관없이 상수버퍼를 하나만 만들고 enum으로 다른 처리를 해주는 방법도 있다
	// 하지만 쉐이더 코드에 if문이 들어가는 것이 좋지 않은 것 같기 때문에..
	// 더 좋은 방법이 있을까?

	CreateConstantBuffer(_directionalLight.data(), _directionalLight.size());
	CreateConstantBuffer(_pointLight.data(), _pointLight.size());
	CreateConstantBuffer(_spotLight.data(), _spotLight.size());
	CreateConstantBuffer(&_lightingData, 1);
	
	// TODO: 상수 버퍼에 데이터가 하나도 없을 경우 생성이 안되는 문제가 있다
}

void Pg::Graphics::RenderObjectLightList::ClearLightData()
{
	_directionalLight.clear();
	_pointLight.clear();
	_spotLight.clear();
}

void Pg::Graphics::RenderObjectLightList::Update(Pg::Data::CameraData* camData)
{
	_lightingData.camPosW = camData->_position;

	if (_input->GetKeyDown(API::Input::NextRenderTarget))
	{
		_lightingData.bufferToRender = (_lightingData.bufferToRender + 1) % 8;
	}
	else if (_input->GetKeyDown(API::Input::PrevRenderTarget))
	{
		_lightingData.bufferToRender = (_lightingData.bufferToRender -1) % 8;
	}
}
