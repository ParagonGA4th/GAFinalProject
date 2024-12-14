#pragma once
#include "RenderObject2D.h"
#include "../ParagonData/GameObject.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

/// <summary>
/// Paragon Renderer�� Render�Ǵ� ������Ʈ���� ��� ���� �뵵��,
/// ������� �����̳ʿ� Ŭ���� (2D)
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
			//���� ���ٸ�, ���� �ʿ䰡 ����.
			if (_list.empty())
			{
				return;
			}

			//����.
			_list.erase(std::remove_if(_list.begin(), _list.end(), [&obj](auto& tPair) { return tPair.first == obj; }));
		}

		std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject2D>>> _list;

	};
}