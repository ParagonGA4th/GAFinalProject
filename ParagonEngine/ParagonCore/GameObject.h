#pragma once

#include "IComponent.h"
#include <unordered_map>
#include <string>

/// <summary>
/// 게임오브젝트 클래스. 
/// 여기가 중요한데... 천천히 공부해서 추가하자!
/// 2023.09.11
/// </summary>
namespace Pg::Core
{
	class GameObject
	{
	public:
		GameObject(const std::string& name);
		virtual ~GameObject();

		void Awake();
		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();

		void OnCollisionEnter();
		void OnCollisionStay();
		void OnCollisionExit();

	private:

		std::string _objectName;
		std::string _objectTag;

		bool _isActive;
	};
}

