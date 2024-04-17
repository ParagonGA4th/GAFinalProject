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
		//static const unsigned int FIXED_UPDATE_ELAPSED_MILLISEC; //근데 우리는 싱글쓰레드 단일 호출이고, 물리 루프 따로 아니라서 상관없을 것 같다.
	};


	__declspec(selectany) const unsigned int GameConstantData::WIDTH = 1920;
	__declspec(selectany) const unsigned int GameConstantData::HEIGHT = 1080;
	//__declspec(selectany) const unsigned int GameConstantData::FIXED_UPDATE_ELAPSED_MILLISEC = 20; // Unity랑 같은 0.02초.

	

}
