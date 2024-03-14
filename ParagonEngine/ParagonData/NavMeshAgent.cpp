#include "NavMeshAgent.h"
#include "GameObject.h"

namespace Pg::Data
{

	NavMeshAgent::NavMeshAgent(GameObject* owner) :
		Component(owner)
	{

	}

	void NavMeshAgent::Update()
	{

	}

	void NavMeshAgent::SetMaxSpeed(float speed)
	{
		_maxSpeed = speed;
	}

	void NavMeshAgent::SetRadius(float rad)
	{
		_radius = rad;
	}

	void NavMeshAgent::SetHeight(float height)
	{
		_height = height;
	}

	float NavMeshAgent::GetMaxSpeed()
	{
		return _maxSpeed;
	}

	float NavMeshAgent::GetRadius()
	{
		return _radius;
	}

	float NavMeshAgent::GetHeight()
	{
		return _height;
	}

}
