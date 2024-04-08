#pragma once
#include "RenderObject2D.h"
#include "../ParagonData/GameObject.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

/// <summary>
/// Paragon Renderer가 Render되는 오브젝트들을 들고 있을 용도로,
/// 만들어진 컨테이너용 클래스 (2D)
/// </summary>

namespace Pg::Graphics
{
	struct RenderObject2DList
	{
	public:
		std::vector<RenderObject2D*> GetRenderObjectWithGameObject(Pg::Data::GameObject* obj)
		{
			std::vector<RenderObject2D*> tRet;

			for (auto& [go,roU] : _list)
			{
				if (go == obj)
				{
					tRet.push_back(roU.get());
				}
			}

			return tRet;
		}

		void DeleteRenderObjectWithGameObject(Pg::Data::GameObject* obj)
		{
			_list.erase(std::remove_if(_list.begin(), _list.end(), [&obj](auto& tPair) { return tPair.first == obj; }));
		}

		std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject2D>>> _list;

	};
}