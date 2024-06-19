#pragma once

//움직이는 배경 오브젝트들이 상속받는 인터페이스
//개별적인 컴포넌트가 이를 부모 클래스로 받아야 한다.

namespace Pg::DataScript
{
	class IMovableObject
	{
	public:
		//위치를 빼놓고, 다른 상태는 원본으로 돌려줘야 하는 함수.
		virtual void ResetAll() abstract;
	};
}
