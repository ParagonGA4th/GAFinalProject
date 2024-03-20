#include "NavMeshAgent.h"
#include "GameObject.h"

namespace Pg::Data
{

	NavMeshAgent::NavMeshAgent(GameObject* owner) :
		Component(owner),
		_crowd(nullptr)
	{

	}

	void NavMeshAgent::Update()
	{

	}

	void NavMeshAgent::AddAgent()
	{
		//Agent의 속성 부여
		dtCrowdAgentParams ap;
		memset(&ap, 0, sizeof(ap));

		ap.radius = _radius;
		ap.maxSpeed = _maxSpeed;
		ap.height = _height;
		ap.maxAcceleration = _maxAcceleration;
		ap.collisionQueryRange = ap.radius * 12.0f;
		ap.pathOptimizationRange = ap.radius * 30.0f;

		ap.updateFlags = 0;
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

	void NavMeshAgent::SetMaxAcceleration(float accel)
	{
		_maxAcceleration = accel;
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

	float NavMeshAgent::GetMaxAcceleration()
	{
		return _maxAcceleration;
	}

	dtCrowd*& NavMeshAgent::GetCrowd()
	{
		return _crowd;
	}

}
