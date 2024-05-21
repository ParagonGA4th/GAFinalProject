#pragma once

//SceneInformation.
#include "../ParagonData/Light.h"
#include "../ParagonData/DirectionalLight.h"
#include "../ParagonData/SpotLight.h"
#include "../ParagonData/PointLight.h"

#include <d3d11.h>
#include <vector>
#include <algorithm>
#include <DirectXMath.h>
/// <summary>
/// 씬 정보와 연동해서 Scene으로 올라가야 하는 정보들.
/// </summary>

namespace Pg::Graphics
{
	struct SceneInformationList
	{
	public:
		enum { LIGHT_MAX_GPU_PASS_COUNT = 10 };
	public:

		//Light 관련. GPU 딴에는 최대 10개까지.
		std::vector<Pg::Data::DirectionalLight*> _dirLightList;
		std::vector<Pg::Data::SpotLight*> _spotLightList;
		std::vector<Pg::Data::PointLight*> _pointLightList;
		bool _isUseLightmap{ false };

		//현재 제어.
		bool _isVignetteOn{ false };
		bool _isBloomOn{ false };
		bool _isLutOn{ false };
		bool _isMotionBlurOn{ false };
	};
}
