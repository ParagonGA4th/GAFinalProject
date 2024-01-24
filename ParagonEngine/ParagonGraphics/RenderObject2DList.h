#pragma once
#include "RenderObject2D.h"
#include "../ParagonData/GameObject.h"
#include <unordered_map>
#include <memory>

/// <summary>
/// Paragon Renderer가 Render되는 오브젝트들을 들고 있을 용도로,
/// 만들어진 컨테이너용 클래스 (2D)
/// </summary>

namespace Pg::Graphics
{
	struct RenderObject2DList
	{
	public:
		std::unordered_map<Pg::Data::GameObject*, std::unique_ptr<RenderObject2D>> _list;
	
		void Clear()
		{
			_list.clear();
		}
	};
}