#pragma once

namespace Pg::Editor
{
	enum class eEventType
	{
		_NONE,
		_NEWSCENE,
		_OPENSCENE,
		_LOADSCENE,
		_SAVESCENE,
		_NEWPROJECT,
		_OPENPROJECT,
		_SAVEPROJECT,
		_OBJECTDATA,
		_EDITORMODE,
		_EDITORDISABLE,
		_GIZMOTYPE,
	};
}