#include "EventTest.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonUtil/Log.h"

#include <cassert>

namespace Pg::Engine
{
	EventTest::EventTest(Pg::Data::GameObject* owner) :
		Component(owner)
	{

	}

	void EventTest::Start()
	{	
		tButton = _object->GetComponent<Pg::Data::Button>();
		audioSource = _object->GetComponent<Pg::Data::AudioSource>();

		tButton->SetOnClickDownEvent([this]
			{
				this->audioSource->Stop();
				//PG_TRACE("BGM Stop.");
			});
	}

	void EventTest::Update()
	{
		
	}
}