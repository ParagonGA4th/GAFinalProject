#pragma once

#include "ScriptInterface.h"
#include "BaseEnemyHandler.h"
#include "TransformSimpleStorage.h"
#include "IEnemyBehaviour.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"



/// <summary>
/// Ex. Wasp Body - Wings���,
// Body�� IEnemyBehaviour�� ������,Wings�� Body�� �θ�� �־� �Ѵ�.
// Mimic - Box�� Mimic�� �ڽ��� Box�� �Ǿ�� �ϰ�,
// GolemBody - GolemCrystal�� ����������.
// �׻� �ְ� �Ǵ� ������Ʈ�� IEnemyBehaviour�� ���� �־�� �Ѵ�.
// �ش� ������Ʈ��, �ڽ��� �ֵ����� ���.
/// </summary>

namespace Pg::DataScript
{
	class Stage1EnemyHandler : public ScriptInterface<Stage1EnemyHandler>, public BaseEnemyHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage1EnemyHandler);

	public:
		Stage1EnemyHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;
	private:
		

	private:
		const unsigned int _entireAreaCount{ 3 };

		// Monster TAG / Instance. 
		// Tag���� ����� �����Ѵ�.
		// Area�� ����� ����.
		
	};
}


