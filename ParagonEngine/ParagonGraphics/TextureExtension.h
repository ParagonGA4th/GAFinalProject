#pragma once

///어떤 텍스쳐인지 외적으로 기록할 수 있게 한다.

namespace Pg::Graphics
{
	enum eTextureExtension
	{
		_UNMANAGED = 0,
		_DDS = 1,
		_PNG = 2,
		_TGA = 3,
		_EXR = 4
	};
}
