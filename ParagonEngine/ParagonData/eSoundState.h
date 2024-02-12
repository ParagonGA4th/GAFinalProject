#pragma once

/// <summary>
/// Audio들의 재생의 여부를 구분해주는 클래스.
/// SoundSystem에 넘겨서 사운드들을 관리할 예정이다.
/// 2024.02.08
/// </summary>

namespace Pg::Data
{
	enum class eSoundState
	{
		_NONE,
		_PLAY,
		_STOP,
		_PAUSE
	};
}
