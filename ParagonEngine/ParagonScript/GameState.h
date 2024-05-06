#pragma once

namespace Pg::DataScript
{
	enum class eGameState
	{
		_NOTSTARTED,	// 게임이 처음 시작 되었을 때
		_PLAY,			// 게임이 실행 중일 때
		_PAUSE,			// 게임이 멈췄을 때
		_STOP,			// 게임이 끝났을 때
	};

	enum class ePlayerState
	{
		_LIVE,
		_DIE,
	};
}