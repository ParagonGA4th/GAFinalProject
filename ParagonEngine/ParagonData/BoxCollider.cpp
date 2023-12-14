#include "BoxCollider.h"
#include "../ParagonData/GameObject.h"
//#include "../ParagonData/Transform.h"

namespace Pg::Data
{
	BoxCollider::BoxCollider(Pg::Data::GameObject* owner) :
		DynamicCollider(owner),
		_width(1.0f),
		_height(1.0f),
		_depth(1.0f)
	{

	}


	void BoxCollider::Start()
	{

	}



	void BoxCollider::Update()
	{
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

	float BoxCollider::GetWidth() const
	{ 
		return _width * _scaleOffset.x * _object->_transform.GetScale().x;
	}

	float BoxCollider::GetHeight() const
	{
		return _height * _scaleOffset.y * _object->_transform.GetScale().y;
	}

	float BoxCollider::GetDepth() const
	{
		return _depth * _scaleOffset.z * _object->_transform.GetScale().z;
	}
}