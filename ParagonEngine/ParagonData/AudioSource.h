#pragma once
#include "Component.h"

#include <string>
#include "data_factory.h" // Auto Register�� ���� �ʼ����.

namespace Pg::Engine
{
	class SoundSystem;
}

/// <summary>
/// �������� AudioSource ������Ʈ.
/// void ������ ���� �Ⱦ�!!!!�Ф�
/// 2024.02.06
/// </summary>
namespace Pg::Data
{
	class GameObject;
	struct AudioData;
	enum class eSoundState;

	//														�θ�			�ڽ�		�Ű�����..
	class AudioSource : public Component, Pg::Factory::Data::RegisteredInFactory<Component, AudioSource, GameObject*>
	{
	public:
		AudioSource(GameObject* owner);

		//�ڵ�ȭ�� Auto-Registration �۵� ���� �ʼ�.
		static Component* CreateInstance(GameObject* go) { return new AudioSource(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::AudioSource"; }

		virtual void Start() override;
		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		void Play();

		void Stop();
		
		void Pause();

		//��� ���� �� �� ����.
		void ResetPlayingState();

		void SetAudioName(const std::string& audioName);
		std::string GetAudioName();

		//void SetBGMVolume(float vol);
		//void SetEffectVolume(float vol);

		//float GetBGMVolume();
		//float GetEffectVolume();
	public:
		//��� ���¸� ��Ÿ���� Ŭ����.
		//������ API���� ���ٲ���.
		AudioData* _audioData;
		eSoundState _soundState;

		BEGIN_VISITABLES(Pg::Data::AudioSource);
		VISITABLE(std::string, _audioName);
		END_VISITABLES;

	private:
		float _bgmVolume;
		float _effectVolume;
	};
}