//#include "RenderObjectLightList.h"
//#include "RenderObjectLight.h"
//#include "../ParagonData/Light.h"
//#include "LowDX11Storage.h"
//
//#include "../ParagonData/Light.h"
//#include "../ParagonData/DirectionalLight.h"
//#include "../ParagonData/PointLight.h"
//#include "../ParagonData/SpotLight.h"
//
//#include "../ParagonData/Transform.h"
//
//#include "../ParagonAPI/PgInput.h"
//
//Pg::Graphics::RenderObjectLightList::RenderObjectLightList()
//	:_DXStorage(LowDX11Storage::GetInstance()), _lightingData()
//{
//	auto& tInputSystem = singleton<Pg::API::Input::PgInput>();
//	_input = &tInputSystem;
//
//	_lightingData.bufferToRender = 7;
//}
//
//void Pg::Graphics::RenderObjectLightList::ParseLights(Pg::Data::Transform* transform, Pg::Data::Light* lightComponent)
//{
//	// ������ ������ ���� �ٸ� ����Ʈ�� �����Ѵ�
//	if (Pg::Data::DirectionalLight* directionalLight = dynamic_cast<Pg::Data::DirectionalLight*>(lightComponent))
//	{
//		Pg::Data::Structs::DirectionalLight* tLightData = new Pg::Data::Structs::DirectionalLight();
//
//		tLightData->position = Pg::Math::PGFLOAT4(transform->GetPosition(), 0.0f);
//
//		tLightData->intensity = directionalLight->GetIntensity();
//		tLightData->color = directionalLight->GetLightColor();
//		tLightData->ambient = directionalLight->GetAmbient();
//		tLightData->diffuse = directionalLight->GetDiffuse();
//		tLightData->specular = directionalLight->GetSpecular();
//
//		tLightData->direction = directionalLight->GetDirection();
//
//		_directionalLight.emplace_back(*tLightData);
//	}
//	else if (Pg::Data::PointLight* pointLight = dynamic_cast<Pg::Data::PointLight*>(lightComponent))
//	{
//		Pg::Data::Structs::PointLight* tLightData = new Pg::Data::Structs::PointLight();
//
//		tLightData->position = Pg::Math::PGFLOAT4(transform->GetPosition(), 0.0f);
//
//		tLightData->intensity = pointLight->GetIntensity();
//		tLightData->color = pointLight->GetLightColor();
//		tLightData->ambient = pointLight->GetAmbient();
//		tLightData->diffuse = pointLight->GetDiffuse();
//		tLightData->specular = pointLight->GetSpecular();
//
//		tLightData->attenuation = pointLight->GetAttenuation();
//		tLightData->range = pointLight->GetRange();
//
//		_pointLight.emplace_back(*tLightData);
//	}
//
//	else if (Pg::Data::SpotLight* SpotLight = dynamic_cast<Pg::Data::SpotLight*>(lightComponent))
//	{
//		Pg::Data::Structs::SpotLight* tLightData = new Pg::Data::Structs::SpotLight();
//		
//		tLightData->position = Pg::Math::PGFLOAT4(transform->GetPosition(), 0.0f);
//
//		tLightData->intensity = SpotLight->GetIntensity();
//		tLightData->color = SpotLight->GetLightColor();
//		tLightData->ambient = SpotLight->GetAmbient();
//		tLightData->diffuse = SpotLight->GetDiffuse();
//		tLightData->specular = SpotLight->GetSpecular();
//
//		tLightData->direction = SpotLight->GetDirection();
//
//		tLightData->attenuation = SpotLight->GetAttenuation();
//		tLightData->range = SpotLight->GetRange();
//
//		_spotLight.emplace_back(*tLightData);
//	}
//
//}
//
//void Pg::Graphics::RenderObjectLightList::BuildConstantBuffers()
//{
//	// ������ ����Ʈ���� ������۸� ������ش�
//	// TODO: ���� ������ ������� ������۸� �ϳ��� ����� enum���� �ٸ� ó���� ���ִ� ����� �ִ�
//	// ������ ���̴� �ڵ忡 if���� ���� ���� ���� ���� �� ���� ������..
//	// �� ���� ����� ������?
//
//	// ������ �����Ͱ� ���� ��� ������۰� ��������� �ʴ� ������ �����ϱ� ����
//	// �����Ͱ� ������ ���� ��� ���� �����͸� �ϳ��� �־��ش�.
//	if (_directionalLight.data() == nullptr)
//	{
//		Pg::Data::Structs::DirectionalLight dummy = { };
//		_directionalLight.emplace_back(dummy);
//	}
//
//	if (_pointLight.data() == nullptr)
//	{
//		Pg::Data::Structs::PointLight dummy = { };
//		_pointLight.emplace_back(dummy);
//	}
//
//	if (_spotLight.data() == nullptr)
//	{
//		Pg::Data::Structs::SpotLight dummy = { };
//		_spotLight.emplace_back(dummy);
//	}
//
//	CreateConstantBuffer(_directionalLight.data(), _directionalLight.size());
//	CreateConstantBuffer(_pointLight.data(), _pointLight.size());
//	CreateConstantBuffer(_spotLight.data(), _spotLight.size());
//	CreateConstantBuffer(&_lightingData, 1);
//}
//
//void Pg::Graphics::RenderObjectLightList::ClearLightData()
//{
//	_directionalLight.clear();
//	_pointLight.clear();
//	_spotLight.clear();
//}
//
//void Pg::Graphics::RenderObjectLightList::Update(Pg::Data::CameraData* camData)
//{
//	_lightingData.camPosW = camData->_position;
//
//	if (_input->GetKeyDown(API::Input::NextRenderTarget))
//	{
//		_lightingData.bufferToRender = (_lightingData.bufferToRender + 1) % 8;
//	}
//	else if (_input->GetKeyDown(API::Input::PrevRenderTarget))
//	{
//		_lightingData.bufferToRender = (_lightingData.bufferToRender -1) % 8;
//	}
//}
//
