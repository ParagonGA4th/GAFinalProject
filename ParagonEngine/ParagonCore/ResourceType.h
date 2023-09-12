#pragma once

/// <summary>
/// 리소스 종류를 구별할 Enum -> 런타임 구별 등에 쓰일 예정!
/// </summary>

namespace Pg::Core::Enum
{
	enum class eResourceType
	{
		//모두 개별적인 클래스가 뒷받침되어야 한다 (IResource를 상속받는)
		_NONE, //기본 상태.
		_3DMODEL,
		_2DTEXTURE,
		_PGPREFAB,
		_PGMATERIAL,
		_PGSHADER,
		_PGFONT,
		_PGSOUND
	};
}
