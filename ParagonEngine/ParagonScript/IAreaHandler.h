#pragma once

//Component의 자식은 아니다. 
//별개로 존재한다. 플로우 관리를 위해.
//인터페이스 활용 다중 상속.
// Area Handler들의 인터페이스.

namespace Pg::DataScript
{
	class IAreaHandler
	{
	public:
		//초기 상태로 되돌리는 함수. 어떤 이유가 되던, 리셋해야 할 때.
		virtual void ResetToInitialState() abstract;

		//구역별 리셋만이 필요할 때. 
		//virtual void ResetAreaUntil(int index) abstract;
		
		//...
	};
}


