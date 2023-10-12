#include "MoveForwardBack.h"
#include "GameObject.h"
#include "Transform.h"

#include <cmath>

namespace Pg::Data
{
	MoveForwardBack::MoveForwardBack(GameObject* obj) : Component(obj)
	{
		//
	}

	void MoveForwardBack::Update()
	{
		tMoveVar += 0.01f;

		Transform& tTrans = this->_object->_transform;
		PGFLOAT3 tCurPos = tTrans.GetPosition();

		tTrans.SetPosition(fmod(tMoveVar, 1.0f) * 4, tCurPos.y, tCurPos.z);
	}

	
	

}