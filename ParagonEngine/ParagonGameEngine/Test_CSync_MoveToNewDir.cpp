#include "Test_CSync_MoveToNewDir.h"
#include "BTHelper.h"

namespace Pg::Engine::BTree::Node
{
	using Pg::Engine::BTree::BTHelper;

	BT::NodeStatus Test_CSync_MoveToNewDir::tick()
	{
		Pg::Data::GameObject* tObject;
		//현재 자신이 속한 오브젝트를 받아왔다. (InputPort)
		BTHelper::Validate(getInput<Pg::Data::GameObject*>(BTree::PRIVATE_OBJECT_KEY, tObject));
		Pg::Math::PGQuaternion tRot;
		BTHelper::Validate(getInput<Pg::Math::PGQuaternion>("_moveDir", tRot));

		//오브젝트의 Rotation에 대입. 
		///tObject->_transform._rotation = tRot;
		
		
		



	}

}
