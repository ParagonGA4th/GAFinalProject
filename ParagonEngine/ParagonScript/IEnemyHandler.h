#pragma once
#include "EnemyDefinesAndStructs.h"
#include <functional>
//Component의 자식은 아니다. 
//별개로 존재한다. 플로우 관리를 위해.
//인터페이스 활용 다중 상속.
//Enemy Handler들의 인터페이스.

namespace Pg::DataScript
{
	class IEnemyHandler
	{
	public:
		//초기 상태로 되돌리는 함수. 어떤 이유가 되던, 리셋해야 할 때.
		virtual void ResetToInitialState() abstract;

		//std::transform처럼, 각 함수에게 해당 함수를 호출시킨다.
		//IEnemyBehaviour를 대상으로 호출되는 것.
		virtual void TransformEachEnemy(std::function<void(IEnemyBehaviour*)> func) abstract;

	public:
		
		


	};
}

