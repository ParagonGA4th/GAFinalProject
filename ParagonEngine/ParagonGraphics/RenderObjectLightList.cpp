#include "RenderObjectLightList.h"
#include "RenderObjectLight.h"
#include "../ParagonData/Light.h"
#include "LowDX11Storage.h"

Pg::Graphics::RenderObjectLightList::RenderObjectLightList()
	:_DXStorage(LowDX11Storage::GetInstance())
{
	_LightConstantBufferStruct = new Pg::Data::Structs::Lights;
	CreateConstantBuffer(_LightConstantBufferStruct);

	//for (int i = 0; i < 10; ++i)
	//{
	//	_LightConstantBufferStruct->directionalLight[i] = Pg::Data::Structs::DirectionalLight();
	//}
	//for (int i = 0; i < 10; ++i)
	//{
	//	_LightConstantBufferStruct->pointLight[i] = Pg::Data::Structs::PointLight();
	//}
}

void Pg::Graphics::RenderObjectLightList::UpdateConstantBuffer()
{
	// TODO: _list 를 순회하며 라이트 데이터들을 상수버퍼에 넣고 업데이트

	//for (auto i = 0; i < _list.size(); ++i)
	//{
	//	if (e->_type == Data::Enums::eLightType::DIRECTIONALLIGHT)
	//	{
	//		_LightConstantBufferStruct->directionalLight[i];
	//	}
	//	else if (e->_type == Data::Enums::eLightType::POINTLIGHT)

	//	{
	//		_LightConstantBufferStruct->pointLight[i];
	//	}
	//}


	for (auto& cb : _constantBuffers)
	{
		cb->UpdateAndBind(_constantBuffers.size());
	}
}