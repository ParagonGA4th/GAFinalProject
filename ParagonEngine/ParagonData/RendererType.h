#pragma once

namespace Pg::Data::Enums
{
	enum class eRendererType : unsigned char
	{
		_SPRITE_RENDERER = 0,
		_TEXT_RENDERER = 1,
		_STATIC_MESH_RENDERER = 2,
		_SKINNED_MESH_RENDERER = 3,
	};
}