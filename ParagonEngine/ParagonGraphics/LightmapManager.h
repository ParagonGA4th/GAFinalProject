#pragma once
#include "LightmapParser.h"
#include "../ParagonData/Scene.h"

#include <string>
#include <memory>
#include <unordered_map>

/// <summary>
/// ����Ʈ�� ���� ��ü�� �ô´�.
/// </summary>

namespace Pg::Graphics
{
	class LightmapManager
	{
	public:
		LightmapManager();
		~LightmapManager();

		void Initialize(const std::string& resourceListPath);

		//���� ���� ������, ���� �� ���. ������, �ƹ� ���۵� ���� ����.
		void SetGPULightmapDataWithScene(const Pg::Data::Scene* scene, void* renderObjectList);
	
		//���� Temp.
		void Temp(const Pg::Data::Scene* scene);

		bool GetIsSceneUseLightmap() { return _isSceneUseLightmap; }
	private:
		void LoadLightmapCSVList(const std::string& resourceListPath);
		
	private:
		std::unordered_map<std::string, std::unique_ptr<RenderLightmapData>> _lightmapStorage;
		std::unique_ptr<LightmapParser> _lightmapParser;
		bool _isSceneUseLightmap{ false };

	};
}


