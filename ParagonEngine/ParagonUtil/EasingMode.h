#pragma once
/// <summary>
/// Tween 사용을 위해, 미리 나열된 EasingMode들.
/// </summary>

namespace Pg::Util::Enums
{
	enum eEasingMode
	{
		LINEAR,
		INBACK,
		OUTBACK,
		INOUTBACK,
		INBOUNCE,
		OUTBOUNCE,
		INOUTBOUNCE,
		INELASTIC,
		OUTELASTIC,
		INOUTELASTIC,
		INSINE,
		OUTSINE,
		INOUTSINE,
		INEXPO,
		OUTEXPO,
		INOUTEXPO,
		INCIRC,
		OUTCIRC,
		INOUTCIRC,
		INQUAD,
		OUTQUAD,
		INOUTQUAD,
		INCUBIC,
		OUTCUBIC,
		INOUTCUBIC,
		INQUART,
		OUTQUART,
		INOUTQUART,
		INQUINT,
		OUTQUINT,
		INOUTQUINT,

		LAST // 열거형의 끝
	};
}