#pragma once
#include <set>
#include <string>
#include <DirectXMath.h>
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
		static const DirectX::XMFLOAT2 WIDTH_HEIGHT_PAIR;
		//static std::set<std::string> INSTANCING_EXCEPTION_MESH_PATH_SET;

		static const unsigned int SHADOW_MAP_LENGTH;
		//static const unsigned int FIXED_UPDATE_ELAPSED_MILLISEC; //근데 우리는 싱글쓰레드 단일 호출이고, 물리 루프 따로 아니라서 상관없을 것 같다.
	};

	__declspec(selectany) const DirectX::XMFLOAT2 GameConstantData::WIDTH_HEIGHT_PAIR = { 2560, 1440 };

	//__declspec(selectany) std::set<std::string> GameConstantData::INSTANCING_EXCEPTION_MESH_PATH_SET{};
	__declspec(selectany) const unsigned int GameConstantData::WIDTH = 2560;
	__declspec(selectany) const unsigned int GameConstantData::HEIGHT = 1440;
	//__declspec(selectany) const unsigned int GameConstantData::FIXED_UPDATE_ELAPSED_MILLISEC = 20; // Unity랑 같은 0.02초.

	__declspec(selectany) const unsigned int GameConstantData::SHADOW_MAP_LENGTH = 2048;
}
