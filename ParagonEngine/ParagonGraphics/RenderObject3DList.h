#pragma once
#include "RenderObject3D.h"
#include "RenderObjectStaticMesh3D.h"
#include "RenderObjectSkinnedMesh3D.h"
#include "RenderMaterial.h"
#include "../ParagonData/GameObject.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

/// <summary>
/// Paragon Renderer가 Render되는 오브젝트들을 들고 있을 용도로,
/// 만들어진 컨테이너용 클래스 (3D)
/// </summary>

namespace Pg::Graphics
{
	struct RenderObject3DList
	{
	public:

		std::vector<RenderObject3D*> GetRenderObjectWithGameObject(Pg::Data::GameObject* obj)
		{
			std::vector<RenderObject3D*> tRet;

			for (auto& it : this->_staticList)
			{
				for (auto& [go, ro] : *(it.second.get()))
				{
					if (go == obj)
					{
						tRet.push_back(ro.get());
					}
				}
			}

			for (auto& it : this->_skinnedList)
			{
				for (auto& [go, ro] : *(it.second.get()))
				{
					tRet.push_back(ro.get());
				}
			}
			return tRet;
		}

		void DeleteRenderObjectWithGameObject(Pg::Data::GameObject* obj)
		{
			//없다면 점검할 필요 조차 없다.

			//하나하나 vector.
			if (!_staticList.empty()) 
			{
				for (auto& [bMat, bVec] : this->_staticList)
				{
					auto tVec = bVec.get();
					if (!tVec->empty())
					{
						tVec->erase(std::remove_if(tVec->begin(), tVec->end(), [&obj](auto& tPair) { return tPair.first == obj; }));
					}
				}
			}
			
			if (!_skinnedList.empty())
			{
				for (auto& [bMat, bVec] : this->_skinnedList)
				{
					auto tVec = bVec.get();
					if (!tVec->empty())
					{
						tVec->erase(std::remove_if(tVec->begin(), tVec->end(), [&obj](auto& tPair) { return tPair.first == obj; }));
					}
				}
			}
		}


		//일단은 Material Index와 관계는 없을 것이다.
		std::unordered_map<std::string, RenderMaterial*> _materialPathSet;

		//FirstPath가 지나면 Static, Skinned의 차이가 없다.
		//FirstStatic/Skinned에 한정해서 차이나게 기록하고, 나머지는 일괄적으로 렌더해버리자.

		//Static
		std::unordered_map<RenderMaterial*, /* Material Path */
			std::unique_ptr<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectStaticMesh3D>>>>> _staticList; /* Vector */

		//Skinned
		std::unordered_map<RenderMaterial*, /* Material Path */
			std::unique_ptr<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectSkinnedMesh3D>>>>> _skinnedList; /* Vector */


	};

}