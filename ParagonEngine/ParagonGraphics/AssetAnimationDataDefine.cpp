#include "AssetAnimationDataDefine.h"
#include "AssetModelDataDefine.h"
#include "../ParagonUtil/CustomAssert.h"

namespace Pg::Graphics
{
	NodeAnim_AssetData::NodeAnim_AssetData()
	{

	}

	NodeAnim_AssetData::~NodeAnim_AssetData()
	{

	}

	BoneInfo_AssetData::BoneInfo_AssetData() :
		_name(), _index(0), _bindedNode(nullptr), _offsetMatrix(DirectX::XMMatrixIdentity())
	{
		//_finalTransformation = DirectX::SimpleMath::Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}

	//VertexBone
	VertexBone_TempAssetData::VertexBone_TempAssetData()
	{
		//아무리 Bone의 영향을 받지 않더라도, 멈추지는 않게 되어 있다.
		memset(IDs, 0, 4 * sizeof(IDs[0]));
		memset(Weights, 0, 4 * sizeof(Weights[0]));
	}

	void VertexBone_TempAssetData::AddBoneData(unsigned int BoneID, float Weight)
	{
		for (unsigned int i = 0; i < 4; i++) {

			// Check to see if there are any empty weight values. 
			if (Weights[i] <= std::numeric_limits<float>::epsilon()) {
				// Add ID of bone. 
				IDs[i] = BoneID;

				// Set the vertex weight influence for this bone ID. 
				Weights[i] = Weight;
				return;
			}
		}
		// should never get here - more bones than we have space for
		assert(0);
	}

	ModifiedNode_SkinnedMesh::ModifiedNode_SkinnedMesh(ModifiedNode_SkinnedMesh* parentNode) : _parentNode(parentNode)
	{
		_relTransform = std::make_unique<Pg::Data::AnimTransform>();
	}

	ModifiedNode_SkinnedMesh::~ModifiedNode_SkinnedMesh()
	{
		
	}

	void ModifiedNode_SkinnedMesh::RecursiveInitFromNode(Node_AssetData* nodeAssetData, std::unordered_map<std::string, const ModifiedNode_SkinnedMesh*>& recordMap)
	{
		//일단, 정보를 옮겨담기 전 정보 기록. Modifiable Node.
		recordMap.insert(std::make_pair(nodeAssetData->_nodeName, this));

		this->_index = nodeAssetData->_index;
		this->_nodeName = nodeAssetData->_nodeName;

		//자체의 오리지널 데이터 (& 행렬) 저장하기.
		this->_originData = nodeAssetData;

		//복사본에!
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Quaternion rotation;
		DirectX::SimpleMath::Vector3 scale;

		DirectX::SimpleMath::Matrix tOffMat = nodeAssetData->_offsetMatrix;
		CustomAssert(tOffMat.Decompose(scale, rotation, position) && "이거 실패하면 Decompose 실패한 것.");

		//Local Transform 세팅, 먼저 만들어져서 들어온다.
		this->_relTransform->SetLocalPosition(position);
		this->_relTransform->SetLocalRotation(rotation);
		this->_relTransform->SetLocalScale(scale);

		this->_numMeshes = nodeAssetData->_numMeshes;

		this->_meshList.resize(nodeAssetData->_meshList.size());
		for (int i = 0; i< nodeAssetData->_meshList.size(); i++)
		{
			//Mesh들의 포인터도 할당.
			this->_meshList[i] = nodeAssetData->_meshList[i];
		}
		this->_bindedBone = nodeAssetData->_bindedBone;

		//이제 재귀적으로 들어갈 차례.
		this->_numChildren = nodeAssetData->_numChildren;

		//Children의 리스트 개수 없으면 리턴.
		if (this->_numChildren == 0)
		{
			return;
		}
		
		//새롭게 만들어진 ChildrenList (기존 node 데이터 복사)
		_childrenList.reserve(_numChildren);

		for (int i = 0; i < this->_numChildren; i++)
		{
			_childrenList.push_back(std::move(std::make_unique<ModifiedNode_SkinnedMesh>(this)));
			
			//자식 노드의 Transform 없는 객체를 그대로 전달해준다. (겜옵젝 없이)
			//AnimTransform 방식으로 , 이에 맞게 넣어준다.
			_childrenList.back()->_relTransform->SetParent(this->_relTransform.get(), false);

			//재귀적으로 기록.
			this->_childrenList.at(i)->RecursiveInitFromNode(nodeAssetData->_childrenList.at(i).get(), recordMap);
		}

		//RecursiveInitFromNode(nodeAssetData->_childrenList.at(i));
	}

}