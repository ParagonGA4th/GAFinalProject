#pragma once
#include "RenderObject3D.h"
//#include "Asset3DModelData.h"
#include "DX11Headers.h"
#include "RenderObjectInstancedMesh3D.h"
#include "RenderObjectStaticMesh3D.h"
#include "RenderObjectSkinnedMesh3D.h"
#include "Rendering3DStructs.h"
#include "RenderMaterial.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/CameraData.h"

#include <unordered_map>
#include <vector>
#include <map>
#include <tuple>
#include <memory>
#include <algorithm>
#include <limits>

/// <summary>
/// Paragon Renderer가 Render되는 오브젝트들을 들고 있을 용도로,
/// 만들어진 컨테이너용 클래스 (3D)
/// </summary>

namespace Pg::Graphics
{
	struct RenderObject3DList
	{
	public:
		//현 프레임에 오브젝트들이 Culling되어야 하는지, 아닌지를 구별. (Update)
		void UpdateObjectCullingState(Pg::Data::CameraData* camData);

		std::vector<RenderObject3D*> GetRenderObjectWithGameObject(Pg::Data::GameObject* obj);
		void DeleteRenderObjectWithGameObject(Pg::Data::GameObject* obj);
		//더 큰 거일수록 뒤에 가야 한다. 
		//먼저 그려져야 하니!
		void SortBlendedByDepth_BackToFront(Pg::Data::CameraData* camData);
		

		//일단은 Material Index와 관계는 없을 것이다. -> 이는 현재로서는 Opaque에 한정.
		std::unordered_map<std::string, RenderMaterial*> _materialPathSet;

		//FirstPath가 지나면 Static, Skinned의 차이가 없다.
		//FirstStatic/Skinned에 한정해서 차이나게 기록하고, 나머지는 일괄적으로 렌더해버리자.

		//Instancing되는 오브젝트들 분리? : 어떻게 할까.. 
		//Material까지 유지하면 좋겠는데. 

		//Instanced Static List. - 일단은 Static만 하자!
		//GameObject 기록할 필요 없을 것. 이미 Transform의 위치가 고정되어 있을 것이기 때문에.
		//기록된 ID3D11Buffer는 인스턴싱에 따로 사용. 이 경우 renderobject의 내부 objmat 버퍼는 채워지지 않는다.
		//일정한 순서가 유지되어야 한다. 내용이 변하지 않으면 같은 순서로 Traversal 할 것이기에.
		//std::map<Asset3DModelData*, std::pair<ID3D11Buffer*, std::unique_ptr<std::vector<InstancedStaticPair>>>> _instancedStaticList;
		std::map<Asset3DModelData*, std::unique_ptr<BufferInstancedPairList>> _instancedStaticList;
		//얘는 반대로 컬링된 요소를 관리하기 위해. (스케일의 마이너스 값이 홀수개일때)
		//std::map<Asset3DModelData*, std::pair<ID3D11Buffer*, std::unique_ptr<std::vector<InstancedStaticPair>>>> _instancedCulledOppositeStaticList;
		std::map<Asset3DModelData*, std::unique_ptr<BufferInstancedPairList>> _instancedCulledOppositeStaticList;

		//그렇다면 얘네들은 유지하되, 인스턴싱되지 않은 애들 기준이어야 한다.
		//Static - Opaque.
		std::unordered_map<RenderMaterial*, /* Material Path */
			std::unique_ptr<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectStaticMesh3D>>>>> _staticList; /* Vector */

		//Skinned - Opaque. 
		std::unordered_map<RenderMaterial*, /* Material Path */
			std::unique_ptr<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectSkinnedMesh3D>>>>> _skinnedList; /* Vector */

		//얘네는 이제 소팅이 되어야 한다.
		//얘는 Forward로 렌더링해야 한다.
		//더 이상 RenderMaterial 단위는 안됨.
		//이제 덮어씌운다! -> 스킨드도 따로 떨어져 있으면 안됨.
		//전에 바인딩된 Material을 보관한 다음에, => 전에 바인딩이 안되었으면 바인딩하는 방식을 쓰자.
		//물리적으로 GameObject의 순서를 바꿔서 소팅하기보다는, 이들의 인덱스를 받아서 인덱스 순서대로 출력하는 방식을 사용하자.
		//중요한게, 오브젝트가 섞여 있다면, 어느 애들은 기존 BlendState로 렌더링하고, 어느 애들은 AlphaBlending 스테이트 대로. 

		//Static & Skinned - AlphaBlended.
		std::vector<std::unique_ptr<AlphaBlendedTuple>> _allAlphaBlendedList;

		//Index를 따로 보관할 것이기에.
		std::vector<int> _sortedIndexBlendedVec;
		///같은 인덱스의 수가 가진 벡터와 대응 순열이 있다. 

	};

}