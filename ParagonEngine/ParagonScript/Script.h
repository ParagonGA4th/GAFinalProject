#pragma once
#include "../ParagonData/Component.h"
#include "../ParagonData/GameObject.h"

//#include <iostream>
#include <memory>
#include <map>
#include <functional>

/// <summary>
/// ������ ��ũ��Ʈ Ŭ����.
/// ����� C++ Scripting�� ������� �Ϸᰡ �Ǿ��⿡, ������ ����ϴ°Ͱ� �и���Ű�� ����.
/// ���Ŀ� GameObject�� �����ϴ� componentList�� scriptList�� �и���ų ����.
/// 2023.10.12
/// </summary>

namespace Pg::DataScript
{
	class Script : public Pg::Data::Component
	{
	public:
		Script(Pg::Data::GameObject* obj) : Component(obj)
		{

		}

		//���� �Ҹ���
		virtual ~Script()
		{

			OnDestroy();
		}

	public:
		virtual void Awake() override {}
		virtual void Start() override {}
		virtual void Update() override {}
		virtual void FixedUpdate() override {}
		virtual void LateUpdate() override {}

		virtual void OnDestroy() override {}
		virtual void OnCollisionEnter(Pg::Data::PhysicsCollision * *_colArr, unsigned int count) override {}
		virtual void OnCollisionStay() override {}
		virtual void OnCollisionExit(Pg::Data::PhysicsCollision** _colArr, unsigned int count) override {}

		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override {}
		virtual void OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count) override {}

		virtual void OnDeserialize(SerializeVector& sv) override {};
		virtual void OnSerialize(SerializeVector& sv) override {};

		virtual void OnAnimationEnd(const std::string& justEndedAnimation) override {};

	public:
		//Scripting System �ڵ�ȭ�� ����.
		using create_instance = std::function<Script*(Pg::Data::GameObject* obj)>;

		//���� Script���� ��ӹ޴� ���� ���´�.
		virtual void DoNotInheritFromShapeDirectlyButFromShapeInterfaceInstead() = 0;

		//Static���� Registry ����. 
		static std::map<std::string, create_instance>& get_registry() {
			static std::map<std::string, create_instance> registry;
			return registry;
		}
	};
}

