#include "SoundSystem.h"
#include "SceneSystem.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/eSoundState.h"
#include "../ParagonData/Transform.h"
#include "../ParagonHelper/rapidcsv.h"
#include "../ParagonHelper/ResourceHelper.h"
#include "../ParagonHelper/CSVHelper.h"

#include <cassert>
#include <algorithm>
#include <filesystem>
#include <singleton-cpp/singleton.h>

namespace Pg::Engine
{
	SoundSystem::SoundSystem() :
		_system(),
		_channelGroup(),
		_channelGroupVec(),
		_soundMap(),
		_maxSound(),
		_maxGroup(3),
		_bgmVolume(0.5f),
		_ingameVolume(0.5f),
		_effectVolume(0.5f)
	{
		//FMOD 초기화
		FMOD::System_Create(&_system);

		//시스템 초기화 (채널 설정을 동시에 해주어야 함)
		_system->init(100, FMOD_INIT_NORMAL, NULL);
		//채널 그룹 초기화
		_system->getMasterChannelGroup(&_channelGroup);


		// FMOD::ChannelGroup 초기화
		for (int i = 0; i < (int)_maxGroup; ++i)
		{
			Pg::Data::eSoundGroup soundGroup = static_cast<Pg::Data::eSoundGroup>(i);

			// 개별 그룹 초기화
			_channelGroupVec[soundGroup] = nullptr;
			std::string groupName = "GroupName" + std::to_string(i);
			_system->createChannelGroup(groupName.c_str(), &_channelGroupVec[soundGroup]);

			_channelGroup->addGroup(_channelGroupVec[soundGroup]);
		}

		//3D 사운드의 크기나 거리 등을 설정
		//_system->set3DSettings(1.0f, 1.0f, 1.0f);

	}

	SoundSystem::~SoundSystem()
	{

	}

	void SoundSystem::Initialize(const std::string& resourceListPath)
	{

		CreateSingleSounds(resourceListPath);

		//얘는 Scene이 바뀔때마다 호출되어야 함.
		SyncAudioSources();
	}


	void SoundSystem::Update()
	{
		//사운드의 재생 상태를 업데이트해서 받아옴.
		UpdateSounds();

		//사운드 들림.
		_system->update();
	}

	void SoundSystem::Finalize()
	{
		//채널 그룹 싹 다 해제.
		for (int i = 0; i < (int)_maxGroup; ++i)
		{
			Pg::Data::eSoundGroup soundGroup = static_cast<Pg::Data::eSoundGroup>(i);
			auto iter = _channelGroupVec.find(soundGroup);

			if (iter != _channelGroupVec.end())
			{
				iter->second->release();
			}
		}

		_channelGroup->release();
		_system->close();
		_system->release();
	}

	void SoundSystem::CreateSound(std::string path, Pg::Data::eSoundGroup soundGroup, bool isLoop)
	{
		//사운드 객체 생성
		Pg::Data::AudioData* audioData = new Pg::Data::AudioData();

		if (!isLoop)
		{
			_system->createSound(path.c_str(), FMOD_LOOP_OFF | FMOD_DEFAULT, NULL, &(audioData->sound));
		}
		else
		{
			_system->createSound(path.c_str(), FMOD_LOOP_NORMAL | FMOD_DEFAULT, NULL, &(audioData->sound));
		}

		audioData->soundPath = path;
		audioData->group = soundGroup;
		audioData->channel = 0;
		audioData->channel->setChannelGroup(_channelGroupVec[soundGroup]);

		_soundMap.insert({ path, audioData });
	}

	void SoundSystem::PlaySound(std::string path)
	{

		_system->playSound(_soundMap.at(path)->sound, NULL, true, &_soundMap.at(path)->channel);
		_soundMap.at(path)->channel->setPaused(false);

		//for (int i = 0; i < (int)_maxSound; i++)
		//{
		//	if (_soundMap.at(path).soundPath == path)
		//	{
		//		_system->playSound(_soundMap.at(path).sound, NULL, true, &_soundMap.at(path).channel);
		//		_soundMap.at(path).channel->setPaused(false);
		//		_soundMap.at(path).isPlaying =
		//			_soundMap.at(path).channel->getCurrentSound(&_soundMap.at(path).sound);
		//	}
		//}
	}


	void SoundSystem::UpdateSounds()
	{
		//무조건 볼륨 설정 후 재생.
		SetAllGroupVolume();

		for (auto& iter : _audioSourceMap)
		{
			Pg::Data::AudioSource*& audioSource = iter.second;
			Pg::Data::AudioData*& audioData = audioSource->_audioData;
			Pg::Data::eSoundState& tSoundState = audioSource->_soundState;

			if (tSoundState == Pg::Data::eSoundState::_PLAY)
			{
				audioData->channel->setPaused(false);

				_system->playSound(audioData->sound, NULL, false, &audioData->channel);

				//소리 중복 재생을 위해 상태를 NONE으로 바꿔줌.
				tSoundState = Data::eSoundState::_NONE;
			}
			else if (tSoundState == Pg::Data::eSoundState::_STOP)
			{
				//_system->stopSound(audioData.sound, NULL, true, &audioData.channel);
				audioData->channel->stop();

				//소리 중복 재생을 위해 상태를 NONE으로 바꿔줌.
				tSoundState = Data::eSoundState::_NONE;
			}
			else if (tSoundState == Pg::Data::eSoundState::_PAUSE)
			{
				audioData->channel->setPaused(true);

				//소리 중복 재생을 위해 상태를 NONE으로 바꿔줌.
				tSoundState = Data::eSoundState::_NONE;
			}

		}
	}


	void SoundSystem::SetAllGroupVolume()
	{
		for (auto iter = _audioSourceMap.begin(); iter != _audioSourceMap.end(); iter++)
		{
			Pg::Data::AudioSource*& audioSource = iter->second;
			Pg::Data::AudioData*& audioData = audioSource->_audioData;

			if (audioData->group == Pg::Data::eSoundGroup::BGM)
			{
				audioData->channel->setVolume(_bgmVolume);
				//_channelGroupVec[soundGroup]->setVolume(audioSource->GetBGMVolume());
			}
			else if (audioData->group == Pg::Data::eSoundGroup::Effect)
			{
				audioData->channel->setVolume(_effectVolume);
			}
			else if (audioData->group == Pg::Data::eSoundGroup::InGame)
			{
				audioData->channel->setVolume(_ingameVolume);
			}
		}
	}

	void SoundSystem::SetAllVolume()
	{
		//사운드 객체 생성
		//for (auto& iter : _soundMap)
		//{
		//	Pg::Data::AudioData& audioData = iter.second;
		//	audioData.channel->setVolume(_audioSource->GetVolume());
		//}

		for (auto& iter : _audioSourceMap)
		{
			Pg::Data::AudioSource*& audioSource = iter.second;
			Pg::Data::AudioData*& audioData = audioSource->_audioData;
			//audioData->channel->setVolume(audioSource->GetVolume());
		}
	}

	void SoundSystem::SoundPause(std::string path, bool isPause)
	{
		_soundMap.at(path)->channel->setPaused(true);
	}

	std::unordered_map<std::string, Pg::Data::AudioData*>& SoundSystem::GetSoundMap()
	{
		return _soundMap;
	}

	void SoundSystem::CreateSingleSounds(const std::string& resourceListPath)
	{
		std::string tUniformPath = Pg::Util::Helper::ResourceHelper::ForcePathUniformFull(resourceListPath);
		std::string tPath = tUniformPath + "/11_Sounds.csv";
		auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromSoundFileCSV(tPath);

		for (auto& [bFilePath, bSoundGroup, bLoopBool] : tPathVec)
		{
			CreateSound(bFilePath, bSoundGroup, bLoopBool);
		}

		//여기다가 전부 사운드 만들어줘야함. 조절은 AudioSource에서.
		///사운드 파일은 엑셀의 csv 파일에 적을 예정.
		//CreateSound("../Resources/Sounds/Test/TitleBGM.mp3", eSoundGroup::BGM, false);
		//CreateSound("../Resources/Sounds/Test/Ingame.mp3", eSoundGroup::BGM, false);
		//CreateSound("../Resources/Sounds/Test/jump.mp3", eSoundGroup::Effect, false);

		_maxSound = _soundMap.size();
	}

	void SoundSystem::SyncAudioSources()
	{
		//원래 있던 AudioSourceList();
		if (!_audioSourceMap.empty())
		{
			for (auto& it : _audioSourceMap)
			{
				it.second->Stop();
			}

			//전 씬에 있던 사운드 멈추기.
			UpdateSounds();

			_audioSourceMap.clear();
		}

		//싱글턴
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		//그냥 다시 찾아라.
		InsertSingleSceneAudioSources(_sceneSystem->GetCurrentScene());

		assert("");
	}

	void SoundSystem::SetBGMVolume(float vol)
	{
		_bgmVolume = vol;
	}

	float SoundSystem::GetBGMVolume()
	{
		return _bgmVolume;
	}

	void SoundSystem::SetIngameVolume(float vol)
	{
		_ingameVolume = vol;
	}

	float SoundSystem::GetIngameVolume()
	{
		return _ingameVolume;
	}

	void SoundSystem::SetEffectVolume(float vol)
	{
		_effectVolume = vol;
	}

	float SoundSystem::GetEffectVolume()
	{
		return _effectVolume;
	}

	void SoundSystem::LoadSoundListOnProjectLoad(const std::vector<Pg::Data::Scene*>& sceneVec)
	{
		//Project Load에 속한 Scene들에 있어, 개별적으로 AudioData를 넣는 것 자체가 중요.
		for (auto& bProjScene : sceneVec)
		{
			InsertSingleSceneAudioSources(bProjScene);
		}

		// 이 자체는 AudioData를 채워넣는 것 자체에 의의가 있었다. 
		// 그러므로, 현재 Scene의 관리 대상이 아니므로, Clear.
		_audioSourceMap.clear();
	}

	void SoundSystem::InsertSingleSceneAudioSources(Pg::Data::Scene* scene)
	{
		for (auto& it : scene->GetObjectList())
		{
			Pg::Data::AudioSource* tAudioSource = it->GetComponent<Pg::Data::AudioSource>();

			if (tAudioSource != nullptr)
			{
				assert(!tAudioSource->GetAudioName().empty() && "AudioSource의 Audio Name이 비워져 있으면 안됨");
				//auto tAudioDataIt = _soundMap.find(tAudioSource->GetAudioName());

				//이제는 이름으로 찾는다.
				std::string name = tAudioSource->GetAudioName();
				Pg::Data::AudioData* tDataFound = nullptr;
				std::string tFullPath = "";
				for (auto& it : _soundMap)
				{
					std::filesystem::path tPath = it.first;
					std::string tVal = tPath.filename().string();

					if (tVal.compare(name) == 0)
					{
						//동일 이름의 리소스가 있을 경우.
						tFullPath = it.first;
						tDataFound = it.second;
					}
				}
				//</>

				assert(tDataFound != nullptr && "SoundMap 내부에서 들어온 String 값의 AudioData를 찾지 못함");

				tAudioSource->_audioData = tDataFound;

				_audioSourceMap.insert(std::make_pair(tFullPath, tAudioSource));
			}

		}
	}

}