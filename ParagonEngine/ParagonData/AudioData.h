#pragma once
#include "eSoundState.h"
#include "../ParagonData/eSoundGroup.h"

#include "../include/fmod/fmod.hpp"
#include "../include/fmod/fmod_common.h"

#include <string>

namespace Pg::Data
{
	/// <summary>
	/// AudioClip 컴포넌트 생성 시 들어갈 구조체.
	/// </summary>
	struct AudioData
	{
		std::string soundPath;	//사운드 파일 경로
		FMOD::Channel* channel;	//사운드 채널
		FMOD::Sound* sound;		//사운드 컨테이너
		Pg::Data::eSoundGroup group;	//사운드 그룹
	};
}