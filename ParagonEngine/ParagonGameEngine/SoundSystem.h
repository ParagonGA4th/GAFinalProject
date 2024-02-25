#pragma once
#include "../ParagonProcess/CoreSingleton.h"

#include "../ParagonData/AudioData.h"
#include "../ParagonData/eSoundGroup.h"
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
/// 2024.01.31
/// </summary>

namespace Pg::Data
{
	class AudioSource;
}

namespace Pg::Engine
{
	class SceneSystem;
	
	class SoundSystem
	{
	public:
		SoundSystem();

		void Initialize();

		void Update();

		void Finalize();

		void CreateSound(std::string path, Pg::Data::eSoundGroup soundGroup, bool isLoop);

		//1차로 작업한 PlaySound. 사실상 지금은 쓰지 않음.
		void PlaySound(std::string path);
		//2차로 작업한 PlaySound.
		void UpdateSounds();

		void SetAllGroupVolume();
		void SetGroupVolume(Pg::Data::eSoundGroup soundGroup);
		void SetAllVolume();

		void SoundPause(std::string path, bool isPause);

		void SyncAudioSources();
	public:
		std::unordered_map<std::string, Pg::Data::AudioData*>& GetSoundMap();

	private:
		void CreateSingleSounds();

	private:
		FMOD::System* _system;
		FMOD::ChannelGroup* _channelGroup;
		std::unordered_map<Pg::Data::eSoundGroup, FMOD::ChannelGroup*> _channelGroupVec;

		std::unordered_map<std::string, Pg::Data::AudioData*> _soundMap;
		std::unordered_map<std::string, Pg::Data::AudioSource*> _audioSoureceMap;

		unsigned int _maxSound;		//사운드 최대 갯수
		unsigned int _maxGroup;		//사운드 그룹 최대 갯수

		bool _isPlay;				//재생 여부
		bool _isLoop;				//빈족 여부

	private:

		SceneSystem* _sceneSystem = nullptr;


		//3D 사운드를 위한 변수
		//3D 사운드 필요없다고 판단.
		//FMOD_VECTOR _soundObjectPos;
		//FMOD_VECTOR _listenerObjectPos;
	};
}

