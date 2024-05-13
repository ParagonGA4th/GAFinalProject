#include "NavMeshAgent.h"
#include "GameObject.h"


namespace Pg::Data
{

	NavMeshAgent::NavMeshAgent(GameObject* owner) :
		Component(owner),
		_radius(1.0f),
		_height(1.0f),
		_maxSpeed(5.0f),
		_maxAcceleration(1.0f),
		_obstacleAvoidance(false),
		_separation(false),
		_separationWeight(2.0f)
	{

	}


	NavMeshAgent::~NavMeshAgent()
	{
		//std::bind를 통해 제거된 Agent를 전달
	}


	void NavMeshAgent::Start()
	{
		//SetDestination({ 0.0f, 0.0f, 0.0f });
	}

	void NavMeshAgent::Update()
	{
		//SetRelocate({ -10.0f, 1.0f, 0.0f });
		//SetDestination({ 0.0f, 0.0f, 0.0f });
	}

	void NavMeshAgent::SetMaxSpeed(float speed)
	{
		_maxSpeed = speed;

	}

	void NavMeshAgent::SetRadius(float rad)
	{
		_radius = rad;

		///런타임에 설정값이 변경될 때 필요함.
		//_updateSystemFunc(this);
	}

	void NavMeshAgent::SetHeight(float height)
	{
		_height = height;
	}

	void NavMeshAgent::SetMaxAcceleration(float accel)
	{
		_maxAcceleration = accel;
	}

	void NavMeshAgent::SetObstacleAvoidance(bool ob)
	{
		_obstacleAvoidance = ob;
	}

	void NavMeshAgent::SetSeperation(bool sep)
	{
		_separation = sep;
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

	bool NavMeshAgent::GetObstacleAvoidance()
	{
		return _obstacleAvoidance;
	}

	bool NavMeshAgent::GetSeparation()
	{
		return _separation;
	}
}
