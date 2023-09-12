#pragma once

namespace Pg::Core::Enums
{
	enum class eResourceType
	{
		//모두 개별적인 클래스가 뒷받침되어야 한다 (IResource를 상속받는)
		_NONE, //기본 상태.
		_3DMODEL,
		_2DTEXTURE,
		_PGMATERIAL,
		_PGSHADER,
		_PGFONT,
		_PGSOUND
	};
}
