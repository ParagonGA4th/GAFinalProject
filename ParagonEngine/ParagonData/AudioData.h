#pragma once
#include "eSoundState.h"
#include "../ParagonData/eSoundGroup.h"

#include "../include/fmod/fmod.hpp"
#include "../include/fmod/fmod_common.h"

#include <string>

namespace Pg::Data
{
	/// <summary>
	/// AudioClip ������Ʈ ���� �� �� ����ü.
	/// </summary>
	struct AudioData
	{
		std::string soundPath;	//���� ���� ���
		FMOD::Channel* channel;	//���� ä��
		FMOD::Sound* sound;		//���� �����̳�
		Pg::Data::eSoundGroup group;	//���� �׷�
	};
}