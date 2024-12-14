#include "RayCast.h"
#include "GameObject.h"


#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

namespace Pg::Data
{
	RayCast::RayCast(GameObject* owner) :
		Collider(owner),
		_origin(0.0f,0.0f,0.0f),
		_dir(1.0f,0.0f,0.0f),
		_length(5.0f)
	{

	}

	void RayCast::Update()
	{
		//RayCast들의 길이를 갱신해준다.
		_rayCastInfo.origin = GetOrigin();
		_rayCastInfo.dir = GetDir();
		_rayCastInfo.length = GetLength();
	}

	void RayCast::SetOrigin(PGFLOAT3 origin)
	{
		_origin = origin;
	}

	void RayCast::SetDir(PGFLOAT3 dir)
	{
		_dir = dir;
	}

	void RayCast::SetLength(float length)
	{
		_length = length;
	}

	Pg::Math::PGFLOAT3 RayCast::GetOrigin()
	{
		return _origin;
	}

	Pg::Math::PGFLOAT3 RayCast::GetDir()
	{
		return _dir;
	}

	float RayCast::GetLength()
	{
		return _length;
	}

	physx::PxScene* RayCast::GetPxScene()
	{
		return _scene;
	}

	void RayCast::SetPxScene(physx::PxScene* scene)
	{
		_scene = scene;
	}

	void RayCast::UpdateTransform()
	{

	}
}