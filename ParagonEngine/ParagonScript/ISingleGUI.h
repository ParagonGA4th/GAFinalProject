#pragma once

///모든 개별적인 GUI 요소가 상속받을 인터페이스.

namespace Pg::DataScript
{
	class ISingleGUI
	{
	public:
		//필요한 만큼 값을 받아서 쓰는 요소이다.
		//Ex. Player의 Health라던가, 필요한 요소를 Pointer로 받아서 쓸 수 있게 한다.
		//이는 Handler에서 일괄적으로 Setup 과정에서 전달하게 될 것이다.
		//해당 정보를 해석하는 양상은 개별 클래스마다 달라지게 될 것이다.
		//해당 Script는 Renderer2D에 접속해서, FillRatio, Index 등등을 바꾸게 된다. 
		virtual void ReceiveDependentPointers(void* p1, void* p2, void* p3, double v1, double v2, double v3) abstract;
	};
}
