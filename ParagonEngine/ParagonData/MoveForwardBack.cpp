#include "MoveForwardBack.h"
#include "GameObject.h"
#include "Transform.h"

#include <cmath>

namespace Pg::Data
{
	MoveForwardBack::MoveForwardBack(GameObject* obj) : Component(obj)
	{

	}

	void MoveForwardBack::Update()
	{
		tMoveVar += 0.05f;

		Transform& tTrans = this->_object->_transform;
		PGFLOAT3 tCurPos = tTrans.GetPosition();

		tTrans.SetPosition(fmod(cos(tMoveVar), 2.0f) * 10, tCurPos.y, fmod(sin(tMoveVar), 2.0f) * 10);
		tTrans.SetLocalRotationEuler(0.f, -tMoveVar, 0.f);
	}

	
	

}