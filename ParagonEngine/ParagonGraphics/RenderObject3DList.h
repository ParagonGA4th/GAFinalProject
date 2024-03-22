#pragma once
#include "RenderObject3D.h"
#include "../ParagonData/GameObject.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include "RenderMaterial.h"

/// <summary>
/// Paragon Renderer가 Render되는 오브젝트들을 들고 있을 용도로,
/// 만들어진 컨테이너용 클래스 (3D)
/// </summary>

namespace Pg::Graphics
{
	struct RenderObject3DList
	{
	public:
		//일단은 Material Index와 관계는 없을 것이다.
		std::vector<std::pair<std::string, RenderMaterial*>> _materialPathSet;

		//FirstPath가 지나면 Static, Skinned의 차이가 없다.
		//FirstStatic/Skinned에 한정해서 차이나게 기록하고, 나머지는 일괄적으로 렌더해버리자.

		//Static
		std::unordered_map<RenderMaterial*, /* Material Path */
			std::unique_ptr<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject3D>>>>> _staticList; /* Vector */

		//Skinned
		std::unordered_map<RenderMaterial*, /* Material Path */
			std::unique_ptr<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObject3D>>>>> _skinnedList; /* Vector */


	};

}