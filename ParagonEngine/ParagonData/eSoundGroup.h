#pragma once

/// <summary>
/// 여러개의 사운드로 이루어진 그룹.
/// </summary>
namespace Pg::Data
{
	enum class eSoundGroup
	{
		BGM = 0,
		InGame = 1,
		Effect = 2,

		//enum의 갯수
		Count = 3
	};
}
