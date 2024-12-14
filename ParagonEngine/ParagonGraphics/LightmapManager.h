#pragma once
#include "LightmapParser.h"
#include "../ParagonData/Scene.h"

#include <string>
#include <memory>
#include <unordered_map>

/// <summary>
/// 라이트맵 관리 전체를 맡는다.
/// </summary>

namespace Pg::Graphics
{
	class LightmapManager
	{
	public:
		LightmapManager();
		~LightmapManager();

		void Initialize(const std::string& resourceListPath);

		//실제 값이 있으면, 정렬 후 사용. 없으면, 아무 동작도 하지 않음.
		void SetGPULightmapDataWithScene(const Pg::Data::Scene* scene, void* renderObjectList);
	
		//지울 Temp.
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


