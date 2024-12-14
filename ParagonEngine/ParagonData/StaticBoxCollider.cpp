#include "StaticBoxCollider.h"
#include "GameObject.h"
#include <algorithm>

namespace Pg::Data
{

	StaticBoxCollider::StaticBoxCollider(GameObject* owner) :
		StaticCollider(owner),
		_width(2.0f),
		_height(2.0f),
		_depth(2.0f)
	{
	}

	void StaticBoxCollider::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper<StaticBoxCollider>(this, sv);
	}	

	void StaticBoxCollider::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper<StaticBoxCollider>(this, sv);
	}

	void StaticBoxCollider::Update()
	{
		using namespace Pg::Math;

		PGFLOAT4X4 worldTM = _object->_transform.GetWorldTM() * GetOffsetTM();
		PGFLOAT3 Whd = { GetWidth(), GetHeight(), GetDepth() };
		PGFLOAT4 color = { 0.0f,0.0f,1.0f,1.0f };
		_boxInfo.worldTM = worldTM;
		_boxInfo.scale = Whd;
		_boxInfo.color = color;
	}

	float StaticBoxCollider::GetWidth() const
	{
		//Collider 기록에 의해 0에 가깝게 기록되는 경우 막기 위해 max 투입.
		//return _width * _scaleOffset.x * fabs(_object->_transform._scale.x);
		return _width * _scaleOffset.x * std::max(fabs(_object->_transform._scale.x), 0.001f);
	}

	float StaticBoxCollider::GetHeight() const
	{
		//Collider 기록에 의해 0에 가깝게 기록되는 경우 막기 위해 max 투입.
		return _height * _scaleOffset.y * std::max(fabs(_object->_transform._scale.y), 0.001f);
	}

	float StaticBoxCollider::GetDepth() const
	{
		//Collider 기록에 의해 0에 가깝게 기록되는 경우 막기 위해 max 투입.
		return _depth * _scaleOffset.z * std::max(fabs(_object->_transform._scale.z), 0.001f);
	}

	void StaticBoxCollider::SetScale(float w, float h, float d)
	{
		_width = w;
		_height = h;
		_depth = d;
	}
}