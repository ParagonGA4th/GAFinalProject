#pragma once

namespace Pg::UI
{
	// 기존 위젯과 연결
	// 각 기능 당 값 return -> template로(enum으로 구분)

	class IWidget
	{
	public:
		virtual void Update() abstract;
	};
}

