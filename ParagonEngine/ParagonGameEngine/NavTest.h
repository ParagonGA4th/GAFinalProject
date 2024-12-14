#pragma once
#include "../ParagonData/Component.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// ������ ���� �̺�Ʈ���� �׽�Ʈ�� ��ũ��Ʈ.
/// </summary>

namespace Pg::Engine
{
	class Navigation;
	class DebugSystem;

	class NavTest : public Pg::Data::Component
	{
	public:
		NavTest(Pg::Data::GameObject* owner);

	public:
		virtual void Start() override;
		virtual void Update() override;

		void Move(Pg::Math::PGFLOAT3 startPos, Pg::Math::PGFLOAT3 targetPos);
		void Move2(Pg::Math::PGFLOAT3 startPos, Pg::Math::PGFLOAT3 targetPos);

	private:

		std::vector<std::pair<Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT3>> _straightpath;

		Pg::Engine::Navigation* _navSystem = nullptr;
		Pg::Engine::DebugSystem* _debugSystem = nullptr;
	};
}



