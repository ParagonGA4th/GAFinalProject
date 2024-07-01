#pragma once
#include <string>

//Visual Effect 목록을 이렇게 받는다.

namespace Pg::Data
{
	//Effect에서만 사용될 것이니.
	enum eSpriteMode
	{
		_DEFAULT = 0,
		_SPRITE_SHEET = 1,
		_SCROLLING_BG = 2
	};

	struct VisualEffectData
	{
		std::string _effectName{};					//반영
		bool _is3dSpace{ true };					//반영
		bool _isFaceCamera{ true };					//런타임에 Matrix 업데이트때 활용될 것.
		bool _isAlphaBlended{ false };				//반영
		//0-100까지.
		float _alphaPercentage{ 100.0f };			//현재로서는 미반영.
		std::string _textureName; 					//반영
		eSpriteMode _spriteMode{ _DEFAULT }; // eSpriteMode : Main Texture에 한정. 2번째 텍스쳐부터는 시트가 될 수 없다. 
		unsigned int _frameCount{ 1 };
		bool _isUseCustomVertexShader{ false };		//반영
		std::string _customVertexShaderName{};		//반영
		bool _isUseCustomPixelShader{ false };		//반영
		std::string _customPixelShaderName{};		//반영 
	};
}
