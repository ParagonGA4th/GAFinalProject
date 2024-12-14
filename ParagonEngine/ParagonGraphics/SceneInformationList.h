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
/// �� ������ �����ؼ� Scene���� �ö󰡾� �ϴ� ������.
/// </summary>

namespace Pg::Graphics
{
	struct SceneInformationList
	{
	public:
		enum { LIGHT_MAX_GPU_PASS_COUNT = 10 };
	public:

		//Light ����. GPU ������ �ִ� 10������.
		std::vector<Pg::Data::DirectionalLight*> _dirLightList;
		std::vector<Pg::Data::SpotLight*> _spotLightList;
		std::vector<Pg::Data::PointLight*> _pointLightList;
		bool _isUseLightmap{ false };

		//���� ����.
		bool _isVignetteOn{ false };
		bool _isBloomOn{ false };
		bool _isLutOn{ false };
		bool _isMotionBlurOn{ false };
	};
}
