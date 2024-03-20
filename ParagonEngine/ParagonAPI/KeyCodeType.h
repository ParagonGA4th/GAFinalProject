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
		ShiftR, //일단 Editor Camera 위치 리셋을 위해 만들어놓았음.

		Save,
		CtrlL,
		KeyS,
		KeyZ,

		KeyLeft,
		KeyRight,
		KeyUp,
		KeyDown,
		Space
	};
}
