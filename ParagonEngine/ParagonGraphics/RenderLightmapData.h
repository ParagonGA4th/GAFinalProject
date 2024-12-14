#pragma once
#include "../ParagonData/Scene.h"
#include "Rendering3DStructs.h"
#include <string>
#include <memory>
#include <DirectXMath.h>

/// <summary>
/// ���ҽ� �Ŵ����� ���� ���������� ������, ����Ʈ�ʵ��� �����ؼ� ���� + ����.
/// ���� �ؽ��ĸ� ���� �ִ´�. RenderLightmapData = 1 Scene�� ����.
/// ������ ����Ʈ �����̱� ������.
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
		//Array ������ �ε��� = Lightmap ID.
		std::unique_ptr<RenderTexture2DArray> _lightmapTextureArray;
		
		//�ϴ�, �𸮾��� ���, ����Ƽ���� ���̰� ���� �� ������,
		//������ �����س��ٰ�, ������ ��ġ�� �� ������ �־����! 
		// Model Name // Object Name / Lightmap Set.
		std::unordered_map<std::string, std::unordered_map<std::string, SingleLightMapSet>> _beforeAlignMaps;

		

	};
}


