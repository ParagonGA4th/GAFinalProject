#pragma once
#include "../ParagonProcess/CoreSingleton.h"

//FMOD 라이브러리
//#pragma comment(lib, "fmod_vc.lib")

#include "../include/fmod/fmod.h"
#include "../include/fmod/fmod.hpp"
#include "../include/fmod/fmod_common.h"


#include <vector>
#include <string>
/// <summary>
/// 변지상의 SoundSystem
/// FMOD를 기반으로 3D 사운드를 구현할 예정이다.
/// </summary>
namespace Pg::Engine
{
	class SoundSystem
	{
	public:
		SoundSystem();

		void Initialize();

		void Finalize();

		void CreateSound(std::string path);

		void SetVolume(std::string path, float vol);

	private:
		FMOD::System* _system;
		FMOD::ChannelGroup* _channelGroup; 

		unsigned int _maxSound;		//사운드 최대 갯수
		unsigned int _maxGroup;		//사운드 그룹 최대 갯수
	};
}

