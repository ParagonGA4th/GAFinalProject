#pragma once
#include <string>

//Visual Effect 목록을 이렇게 받는다.

namespace Pg::Data
{
	struct VisualEffectData
	{
		std::string _effectName{};
		bool _is3dSpace{ true };
		bool _isFaceCamera{ true };
		bool _isAlphaBlended{ false };
		//0-100까지.
		float _alphaPercentage{ 100.0f };
		std::string _textureName;
		bool _isSpriteSheet{ false }; //Main Texture에 한정. 2번째 텍스쳐부터는 시트가 될 수 없다.
		unsigned int _frameCount{ 1 };
		bool _isUseCustomVertexShader{ false };
		std::string _customVertexShaderName{};
		bool _isUseCustomPixelShader{ false };
		std::string _customPixelShaderName{};
	};
}
