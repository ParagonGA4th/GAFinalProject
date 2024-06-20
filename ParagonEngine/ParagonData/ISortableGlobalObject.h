#pragma once

//Don't Destroy On Load가 의도한 순서에 맞게 업데이트되게 하기 위해,
//Sorting되게 한 것.
//매크로 사용으로, 생성자 투입이 불편해짐.
//다른 방법으로 되게 하기 위해.
//동작하기 위해서, Manager들은 1 오브젝트 당 1 컴포넌트의 형태로 들어가야 한다.

namespace Pg::Data
{
	class ISortableGlobalObject
	{
	public:
		//0이 가장 먼저 와야 한다. 구조를 깨지 않고 Update 순서를 확정짓기 위해.
		virtual unsigned int GetPriorityIndex() abstract;
	};
}


