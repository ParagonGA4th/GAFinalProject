#pragma once
#include "RenderObject3D.h"
#include "../ParagonData/GameObject.h"
#include <unordered_map>
#include <vector>
#include <memory>

/// <summary>
/// Paragon Renderer가 Render되는 오브젝트들을 들고 있을 용도로,
/// 만들어진 컨테이너용 클래스 (3D)
/// </summary>

namespace Pg::Graphics
{
	struct RenderObject3DList
	{
	public:
		std::vector<std::string> _materialPathSet;

		std::unordered_map<std::string, /* Material Path */ 
			std::unique_ptr<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject3D>>>>> _list; /* Vector */
	
	public:
		void Clear()
		{
			_list.clear();
			_materialPathSet.clear();
		}
	};

}