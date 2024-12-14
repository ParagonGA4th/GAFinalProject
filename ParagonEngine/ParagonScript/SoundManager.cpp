#include "SoundManager.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgInput.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{

	SoundManager::SoundManager(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void SoundManager::Awake()
	{
		_ingameBgm = _object->GetComponent<Pg::Data::AudioSource>();
	}

	void SoundManager::Start()
	{
		_ingameBgm->Play();
	}

	void SoundManager::Update()
	{

	}
}