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
/// Paragon Renderer�� Render�Ǵ� ������Ʈ���� ��� ���� �뵵��,
/// ������� �����̳ʿ� Ŭ���� (3D)
/// </summary>

namespace Pg::Graphics
{
	struct RenderObject3DList
	{
	public:
		//�� �����ӿ� ������Ʈ���� Culling�Ǿ�� �ϴ���, �ƴ����� ����. (Update)
		void UpdateObjectCullingState(Pg::Data::CameraData* camData);

		std::vector<RenderObject3D*> GetRenderObjectWithGameObject(Pg::Data::GameObject* obj);
		void DeleteRenderObjectWithGameObject(Pg::Data::GameObject* obj);
		//�� ū ���ϼ��� �ڿ� ���� �Ѵ�. 
		//���� �׷����� �ϴ�!
		void SortBlendedByDepth_BackToFront(Pg::Data::CameraData* camData);
		

		//�ϴ��� Material Index�� ����� ���� ���̴�. -> �̴� ����μ��� Opaque�� ����.
		std::unordered_map<std::string, RenderMaterial*> _materialPathSet;

		//FirstPath�� ������ Static, Skinned�� ���̰� ����.
		//FirstStatic/Skinned�� �����ؼ� ���̳��� ����ϰ�, �������� �ϰ������� �����ع�����.

		//Instancing�Ǵ� ������Ʈ�� �и�? : ��� �ұ�.. 
		//Material���� �����ϸ� ���ڴµ�. 

		//Alpha Clipping Instanced List: Proper and Culled Backwards.
		std::map<Asset3DModelData*, std::unique_ptr<BufferInstancedPairList>> _instancedStaticAlphaClippedList;
		std::map<Asset3DModelData*, std::unique_ptr<BufferInstancedPairList>> _instancedCulledOppositeStaticAlphaClippedList;

		//Instanced Static List. - �ϴ��� Static�� ����!
		//GameObject ����� �ʿ� ���� ��. �̹� Transform�� ��ġ�� �����Ǿ� ���� ���̱� ������.
		//��ϵ� ID3D11Buffer�� �ν��Ͻ̿� ���� ���. �� ��� renderobject�� ���� objmat ���۴� ä������ �ʴ´�.
		//������ ������ �����Ǿ�� �Ѵ�. ������ ������ ������ ���� ������ Traversal �� ���̱⿡.
		//std::map<Asset3DModelData*, std::pair<ID3D11Buffer*, std::unique_ptr<std::vector<InstancedStaticPair>>>> _instancedStaticList;
		std::map<Asset3DModelData*, std::unique_ptr<BufferInstancedPairList>> _instancedStaticList;
		//��� �ݴ�� �ø��� ��Ҹ� �����ϱ� ����. (�������� ���̳ʽ� ���� Ȧ�����϶�)
		//std::map<Asset3DModelData*, std::pair<ID3D11Buffer*, std::unique_ptr<std::vector<InstancedStaticPair>>>> _instancedCulledOppositeStaticList;
		std::map<Asset3DModelData*, std::unique_ptr<BufferInstancedPairList>> _instancedCulledOppositeStaticList;

		//�׷��ٸ� ��׵��� �����ϵ�, �ν��Ͻ̵��� ���� �ֵ� �����̾�� �Ѵ�.
		//Static - Opaque.
		std::unordered_map<RenderMaterial*, /* Material Path */
			std::unique_ptr<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectStaticMesh3D>>>>> _staticList; /* Vector */

		//Skinned - Opaque. 
		std::unordered_map<RenderMaterial*, /* Material Path */
			std::unique_ptr<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectSkinnedMesh3D>>>>> _skinnedList; /* Vector */

		//��״� ���� ������ �Ǿ�� �Ѵ�.
		//��� Forward�� �������ؾ� �Ѵ�.
		//�� �̻� RenderMaterial ������ �ȵ�.
		//���� ������! -> ��Ų�嵵 ���� ������ ������ �ȵ�.
		//���� ���ε��� Material�� ������ ������, => ���� ���ε��� �ȵǾ����� ���ε��ϴ� ����� ����.
		//���������� GameObject�� ������ �ٲ㼭 �����ϱ⺸�ٴ�, �̵��� �ε����� �޾Ƽ� �ε��� ������� ����ϴ� ����� �������.
		//�߿��Ѱ�, ������Ʈ�� ���� �ִٸ�, ��� �ֵ��� ���� BlendState�� �������ϰ�, ��� �ֵ��� AlphaBlending ������Ʈ ���. 

		//Static & Skinned - AlphaBlended.
		std::vector<std::unique_ptr<AlphaBlendedTuple>> _allAlphaBlendedList;

		//Index�� ���� ������ ���̱⿡.
		std::vector<int> _sortedIndexBlendedVec;
		///���� �ε����� ���� ���� ���Ϳ� ���� ������ �ִ�. 

	};

}