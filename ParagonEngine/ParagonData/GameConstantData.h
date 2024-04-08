#pragma once

/// <summary>
/// Data에서 Width / Height 등 내부적으로 필요하고 변하지 않을 데이터 저장 목적.
/// </summary>

namespace Pg::Data
{
	struct GameConstantData
	{
	public:
		static const unsigned int WIDTH;
		static const unsigned int HEIGHT;
	};

	__declspec(selectany) const unsigned int GameConstantData::WIDTH = 1920;
	__declspec(selectany) const unsigned int GameConstantData::HEIGHT = 1080;

	

}
