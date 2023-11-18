#pragma once

/// <summary>
/// Editor의 Mode가 현재 어떻게 되는지 규정하는 Enum.
/// </summary>

namespace Pg::Data::Enums
{
	enum class eEditorMode
	{
		_EDIT, // 멈춘 상태, 에디터 빌드 가능한 상태.
		_PLAY, // 재생하는 상태
		_PAUSE // 일시정지 하는 상태.
	};
}