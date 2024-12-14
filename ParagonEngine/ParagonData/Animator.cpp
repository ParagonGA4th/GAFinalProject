#include "Animator.h"

#include <behaviortree_cpp_v3/bt_factory.h>
//#include "BtNodes/bt_zmq_publisher.h"
#include <behaviortree_cpp_v3/loggers/bt_file_logger.h>

namespace Pg::Data
{
	Animator::Animator(GameObject* owner) : Component(owner),
		_isUniform(false), _isCulled(false)
	{
		//������ Auto-Register�� ���� ���Ǿ�� �ϴ� ��ũ��.
		FACTORY_INIT;

		//�������ڷ� �� �����ϹǷ�, ��������� Heap�� �ֱ�� �ؾ� �Ѵ�.
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