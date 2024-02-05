#pragma once
#include "../ParagonProcess/CoreSingleton.h"

#include "../ParagonData/AudioData.h"

//FMOD 라이브러리
//#pragma comment(lib, "fmod_vc.lib")

#include "../include/fmod/fmod.h"
#include "../include/fmod/fmod.hpp"
#include "../include/fmod/fmod_common.h"


#include <vector>
#include <string>
#include <unordered_map>
/// <summary>
/// 변지상의 SoundSystem
/// FMOD를 기반으로 사운드를 구현할 예정이다.
/// 3D 사운드는 게임 구현에 있어 필요 없다고 판단.
/// 2024.01
/// </summary>
namespace Pg::Engine
{
	class SoundSystem
	{
	public:
		SoundSystem();

		void Initialize();

		void Update();

		void Finalize();

		void CreateSound(std::string path);

		void PlaySound(std::string path, bool isLoop);

		void SetVolume(std::string path, float vol);

		void SoundPause(std::string path);

	private:
		FMOD::System* _system;
		FMOD::ChannelGroup* _channelGroup;
		std::vector<FMOD::ChannelGroup*> _channelGroupVec;

		std::unordered_map<std::string, Pg::Data::AudioData*> _soundMap;

		unsigned int _maxSound;		//사운드 최대 갯수
		unsigned int _maxGroup;		//사운드 그룹 최대 갯수

		bool _isPlay;				//재생 여부
		bool _isLoop;				//빈족 여부

		//3D 사운드를 위한 변수
		//3D 사운드 필요없다고 판단.
		FMOD_VECTOR _soundObjectPos;
		FMOD_VECTOR _listenerObjectPos;
	};
}

