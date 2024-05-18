#pragma once
#include "../ParagonData/Scene.h"
#include "Rendering3DStructs.h"
#include <string>
#include <memory>
#include <DirectXMath.h>

/// <summary>
/// 리소스 매니저에 의해 관리되지는 않지만, 라이트맵들을 연동해서 갖기 + 관리.
/// 여러 텍스쳐를 갖고 있는다. RenderLightmapData = 1 Scene에 대응.
/// 각각이 리스트 느낌이기 때문에.
/// </summary>

namespace Pg::Graphics
{
	class RenderTexture2D;
	class RenderTexture2DArray;
}

namespace Pg::Graphics
{
	class RenderLightmapData
	{
		friend class LightmapParser;
		friend class LightmapManager;
	public:
		RenderLightmapData(const std::string& lightmapPath, const std::string& sceneName);
		~RenderLightmapData();

	private:
		std::string _path;
		std::string _sceneName;
		bool _isAligned{ false };
		Pg::Data::Scene* _belongScene{ nullptr };
	private:
		//Array 내부의 인덱스 = Lightmap ID.
		std::unique_ptr<RenderTexture2DArray> _lightmapTextureArray;
		
		//일단, 언리얼의 출력, 유니티와의 차이가 있을 수 있으니,
		//별도로 보관해놨다가, 실제로 일치할 때 별도로 넣어놓자! 
		// Model Name // Object Name / Lightmap Set.
		std::unordered_map<std::string, std::unordered_map<std::string, SingleLightMapSet>> _beforeAlignMaps;

		

	};
}


