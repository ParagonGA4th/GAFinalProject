#pragma once
#pragma once
#include "../ParagonData/Component.h"

/// <summary>
/// 구현한 각종 이벤트들을 테스트할 스크립트.
/// </summary>

namespace Pg::Engine
{
	class Navigation;

	class NavTest : public Pg::Data::Component
	{
	public:
		NavTest(Pg::Data::GameObject* owner);

	public:
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::Engine::Navigation* _navSystem = nullptr;
	};
}



