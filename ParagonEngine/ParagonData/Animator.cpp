#include "Animator.h"

#include <behaviortree_cpp_v3/bt_factory.h>
//#include "BtNodes/bt_zmq_publisher.h"
#include <behaviortree_cpp_v3/loggers/bt_file_logger.h>

namespace Pg::Data
{
	Animator::Animator(GameObject* owner) : Component(owner),
		_isUniform(false), _isCulled(false)
	{
		//무조건 Auto-Register를 위해 사용되어야 하는 매크로.
		FACTORY_INIT;

		//역참조자로 값 전달하므로, 명시적으로 Heap에 있기는 해야 한다.
		_behavTree = new BT::Tree();
	}

	Animator::~Animator()
	{
		if (_behavTree != nullptr)
		{
			delete _behavTree;
		}
		if (_logger != nullptr)
		{
			delete _logger;
		}

	}

	void Animator::SetBehaviorTreePath(const std::string& path)
	{
		_behaviorTreePath = path;
	}

	void Animator::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void Animator::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void Animator::MonitorSelfInGroot()
	{
		_logger = new BT::FileLogger(*_behavTree, "bt_trace.fbl");
	}

}