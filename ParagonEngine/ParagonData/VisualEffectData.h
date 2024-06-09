#pragma once
#include <string>

namespace Pg::Data
{
	struct VisualEffectData
	{
		std::string _effectName{};
		bool _is3dSpace{ true };
		bool _isFaceCamera{ true };
		bool _isAlphaBlended{ false };
		//0-100±îÁö.
		float _alphaPercentage{ 100.0f };
		std::string _textureName;
		bool _isGIF{ false };
	};
}
