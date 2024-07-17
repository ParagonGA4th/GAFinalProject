#pragma once

namespace Pg::API::Input
{
	enum eKeyCode
	{
		MoveFront,
		MoveBack,
		MoveLeft,
		MoveRight,
		KeyE,
		KeyQ,

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
		KeyX,
		KeyZ,

		KeyLeft,
		KeyRight,
		KeyUp,
		KeyDown,
		Space,

		KeyB,
		KeyN,
		KeyM,
		KeyF,
		Esc,

		KeyBackSlash,

		KeyNum1,
		KeyNum2,
		KeyNum3,
		KeyNum4, 
		KeyNum5,
		KeyNum6,
		KeyNum7,
		KeyNum8,
		KeyNum9,
		KeyNum0,

		KeyR
	};
}
