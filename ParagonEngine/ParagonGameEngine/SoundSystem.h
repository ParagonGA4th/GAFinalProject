#pragma once
#include "../ParagonProcess/CoreSingleton.h"

#include "../ParagonData/AudioData.h"
#include "../ParagonData/eSoundGroup.h"

#include <fmod/fmod.h>
#include <fmod/fmod.hpp>
#include <fmod/fmod_common.h>

#include <vector>
#include <string>
#include <unordered_map>

#include "EngineDLLExporter.h"

/// <summary>
/// �������� SoundSystem
/// FMOD�� ������� ���带 ������ �����̴�.
/// 3D ����� ���� ������ �־� �ʿ� ���ٰ� �Ǵ�.
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
		PARAGON_ENGINE_DLL SoundSystem();
		PARAGON_ENGINE_DLL ~SoundSystem();

	public:
		void Initialize(const std::string& resourceListPath);

		void Update();

		void Finalize();

		void CreateSound(std::string path, Pg::Data::eSoundGroup soundGroup, bool isLoop);

		//1���� �۾��� PlaySound. ��ǻ� ������ ���� ����.
		void PlaySound(std::string path);
		//2���� �۾��� PlaySound.
		void UpdateSounds();

		void SetAllGroupVolume();
		void SetAllVolume();

		PARAGON_ENGINE_DLL void SetBGMVolume(float vol);
		PARAGON_ENGINE_DLL void SetIngameVolume(float vol);
		PARAGON_ENGINE_DLL void SetEffectVolume(float vol);

		PARAGON_ENGINE_DLL float GetBGMVolume();
		PARAGON_ENGINE_DLL float GetIngameVolume();
		PARAGON_ENGINE_DLL float GetEffectVolume();

		void SoundPause(std::string path, bool isPause);

		void SyncAudioSources();
	public:
		std::unordered_map<std::string, Pg::Data::AudioData*>& GetSoundMap();
		
		void LoadSoundListOnProjectLoad(const std::vector<Pg::Data::Scene*>& sceneVec);

	private:
		void CreateSingleSounds(const std::string& resourceListPath);

		//������ ���.
		void InsertSingleSceneAudioSources(Pg::Data::Scene* scene);

	private:
		FMOD::System* _system;
		FMOD::ChannelGroup* _channelGroup;
		std::unordered_map<Pg::Data::eSoundGroup, FMOD::ChannelGroup*> _channelGroupVec;

		std::unordered_map<std::string, Pg::Data::AudioData*> _soundMap;
		std::unordered_map<std::string, Pg::Data::AudioSource*> _audioSourceMap;

		unsigned int _maxSound;		//���� �ִ� ����
		unsigned int _maxGroup;		//���� �׷� �ִ� ����

		bool _isPlay;				//��� ����
		bool _isLoop;				//���� ����

	private:

		SceneSystem* _sceneSystem = nullptr;

		//�� SoundGroup�� ���� ����.
		float _bgmVolume;
		float _ingameVolume;
		float _effectVolume;

		//3D ���带 ���� ����
		//3D ���� �ʿ���ٰ� �Ǵ�.
		//FMOD_VECTOR _soundObjectPos;
		//FMOD_VECTOR _listenerObjectPos;
	};
}

