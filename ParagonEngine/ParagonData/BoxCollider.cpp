#include "BoxCollider.h"
#include "GameObject.h"
//#include "../ParagonData/Transform.h"

namespace Pg::Data
{
	BoxCollider::BoxCollider(Pg::Data::GameObject* owner) :
		DynamicCollider(owner),
		_width(1.0f),
		_height(1.0f),
		_depth(1.0f),
		_layer(0)
	{
		FACTORY_INIT;
	}

	void BoxCollider::Update()
	{
		//디버그를 위한 업데이트
		PGFLOAT4X4 worldTM = GetOffsetTM() * _object->_transform.GetWorldTM();
		PGFLOAT3 Whd = { GetWidth(), GetHeight(), GetDepth() };
		PGFLOAT4 color = { 0.0f,1.0f,0.0f,1.0f };
		_boxInfo.worldTM = worldTM;
		_boxInfo.scale = Whd;
		_boxInfo.color = color;
	}

	void BoxCollider::SetScale(float w, float h, float d)
	{
		_width = w;
		_height = h;
		_depth = d;
	}

	void BoxCollider::SetLayer(int lay)
	{
		_layer = lay;
	}

	float BoxCollider::GetWidth() const
	{ 
		return _width * _scaleOffset.x * _object->_transform._scale.x;
	}

	float BoxCollider::GetHeight() const
	{
		return _height * _scaleOffset.y * _object->_transform._scale.y;
	}

	float BoxCollider::GetDepth() const
	{
		return _depth * _scaleOffset.z * _object->_transform._scale.z;
	}

	uint32_t BoxCollider::GetLayer() const
	{
		return _layer;
	}
}