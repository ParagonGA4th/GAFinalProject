#include "StaticBoxCollider.h"
#include "GameObject.h"

namespace Pg::Data
{

	StaticBoxCollider::StaticBoxCollider(GameObject* owner) :
		StaticCollider(owner),
		_width(1.0f),
		_height(1.0f),
		_depth(1.0f)
	{

	}

	void StaticBoxCollider::Update()
	{
		using namespace Pg::Math;

		PGFLOAT4X4 worldTM = GetOffsetTM() * _object->_transform.GetWorldTM();
		PGFLOAT3 Whd = { GetWidth(), GetHeight(), GetDepth() };
		PGFLOAT4 color = { 0.0f,0.0f,1.0f,1.0f };
		_boxInfo.worldTM = worldTM;
		_boxInfo.scale = Whd;
		_boxInfo.color = color;
	}

	float StaticBoxCollider::GetWidth() const
	{
		return _width * _scaleOffset.x * _object->_transform._scale.x;
	}

	float StaticBoxCollider::GetHeight() const
	{
		return _height * _scaleOffset.y * _object->_transform._scale.y;
	}

	float StaticBoxCollider::GetDepth() const
	{
		return _depth * _scaleOffset.z * _object->_transform._scale.z;
	}

	void StaticBoxCollider::SetScale(float w, float h, float d)
	{
		_width = w;
		_height = h;
		_depth = d;
	}
}