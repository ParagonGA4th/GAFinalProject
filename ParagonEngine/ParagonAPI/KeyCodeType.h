#pragma once

namespace Pg::API::Input
{
	enum eKeyCode
	{
		MoveFront,
		MoveBack,
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,

		ButtonMenu,
		ButtonConfirm,

		MouseX,
		MouseY,
		MouseLeft,
		MouseRight,

		EditorOnOff,
		DebugOnOff,

		NextRenderTarget,
		PrevRenderTarget,

		ShiftL, //이동 속도 조정 위해.

		Save,

		CtrlL,
		KeyS,

		KeyLeft,
		KeyRight,
		KeyUp,
		KeyDown,
		Space
	};
}
