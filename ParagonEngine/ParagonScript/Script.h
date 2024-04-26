#pragma once
#include "../ParagonData/Component.h"
#include "../ParagonData/GameObject.h"

//#include <iostream>
#include <memory>
#include <map>
#include <functional>

/// <summary>
/// 엔진의 스크립트 클래스.
/// 현재는 C++ Scripting이 어느정도 완료가 되었기에, 유저가 사용하는것과 분리시키기 위함.
/// 추후에 GameObject에 존재하는 componentList와 scriptList를 분리시킬 예정.
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

		//가상 소멸자
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

		virtual void OnTriggerEnter(Pg::Data::Collider* c) override {}
		virtual void OnTriggerExit(Pg::Data::Collider* c) override {}

	public:
		//Scripting System 자동화를 위해.
		using create_instance = std::function<Script*(Pg::Data::GameObject* obj)>;

		//직접 Script에서 상속받는 것을 막는다.
		virtual void DoNotInheritFromShapeDirectlyButFromShapeInterfaceInstead() = 0;

		//Static으로 Registry 관리. 
		static std::map<std::string, create_instance>& get_registry() {
			static std::map<std::string, create_instance> registry;
			return registry;
		}
	};
}

