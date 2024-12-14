#pragma once
#include <string>

//Visual Effect ����� �̷��� �޴´�.

namespace Pg::Data
{
	//Effect������ ���� ���̴�.
	enum eSpriteMode
	{
		_DEFAULT = 0,
		_SPRITE_SHEET = 1,
		_SCROLLING_BG = 2
	};

	struct VisualEffectData
	{
		std::string _effectName{};					//�ݿ�
		bool _is3dSpace{ true };					//�ݿ�
		bool _isFaceCamera{ true };					//��Ÿ�ӿ� Matrix ������Ʈ�� Ȱ��� ��.
		bool _isAlphaBlended{ false };				//�ݿ�
		//0-100����.
		float _alphaPercentage{ 100.0f };			//����μ��� �̹ݿ�.
		std::string _textureName; 					//�ݿ�
		eSpriteMode _spriteMode{ _DEFAULT }; // eSpriteMode : Main Texture�� ����. 2��° �ؽ��ĺ��ʹ� ��Ʈ�� �� �� ����. 
		unsigned int _frameCount{ 1 };
		bool _isUseCustomVertexShader{ false };		//�ݿ�
		std::string _customVertexShaderName{};		//�ݿ�
		bool _isUseCustomPixelShader{ false };		//�ݿ�
		std::string _customPixelShaderName{};		//�ݿ� 
		bool _manualSwitchingMode{ false }; //���� 3d billboard animation�� ���, ���� ĭ���� �ѱ�� ���� �÷��̾��� �������� ����Ұ��ΰ�?
		bool _ignoreDepth{ false }; //�⺻������ Depth Stencil�� ������ ���� ���ΰ�, �� ���� ���ΰ�?
		bool _isQuadLyingDown{ false }; //����Ʈ�� �����ΰ�, ���� �ִ� ���ΰ�??
	};
}
