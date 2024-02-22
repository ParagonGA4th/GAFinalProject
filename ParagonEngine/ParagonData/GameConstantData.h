#pragma once

/// <summary>
/// Data에서 Width / Height 등 내부적으로 필요하고 변하지 않을 데이터 저장 목적.
/// </summary>

namespace Pg::Data
{
	struct GameConstantData
	{
	public:
		static unsigned int WIDTH;
		static unsigned int HEIGHT;
	};

	__declspec(selectany) unsigned int GameConstantData::HEIGHT = 1920;
	__declspec(selectany) unsigned int GameConstantData::WIDTH = 1080;

}
