#pragma once

namespace Pg::Editor
{
	enum class eEventType
	{
		NONE,
		FileSave,
		FileOpen,
		ChangeObjectData,
	};
}