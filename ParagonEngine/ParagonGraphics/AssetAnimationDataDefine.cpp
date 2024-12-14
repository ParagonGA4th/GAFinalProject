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
		//�ƹ��� Bone�� ������ ���� �ʴ���, �������� �ʰ� �Ǿ� �ִ�.
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
		//�ϴ�, ������ �Űܴ�� �� ���� ���. Modifiable Node.
		recordMap.insert(std::make_pair(nodeAssetData->_nodeName, this));

		this->_index = nodeAssetData->_index;
		this->_nodeName = nodeAssetData->_nodeName;

		//��ü�� �������� ������ (& ���) �����ϱ�.
		this->_originData = nodeAssetData;

		//���纻��!
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Quaternion rotation;
		DirectX::SimpleMath::Vector3 scale;

		DirectX::SimpleMath::Matrix tOffMat = nodeAssetData->_offsetMatrix;
		CustomAssert(tOffMat.Decompose(scale, rotation, position) && "�̰� �����ϸ� Decompose ������ ��.");

		//Local Transform ����, ���� ��������� ���´�.
		this->_relTransform->SetLocalPosition(position);
		this->_relTransform->SetLocalRotation(rotation);
		this->_relTransform->SetLocalScale(scale);

		this->_numMeshes = nodeAssetData->_numMeshes;

		this->_meshList.resize(nodeAssetData->_meshList.size());
		for (int i = 0; i< nodeAssetData->_meshList.size(); i++)
		{
			//Mesh���� �����͵� �Ҵ�.
			this->_meshList[i] = nodeAssetData->_meshList[i];
		}
		this->_bindedBone = nodeAssetData->_bindedBone;

		//���� ��������� �� ����.
		this->_numChildren = nodeAssetData->_numChildren;

		//Children�� ����Ʈ ���� ������ ����.
		if (this->_numChildren == 0)
		{
			return;
		}
		
		//���Ӱ� ������� ChildrenList (���� node ������ ����)
		_childrenList.reserve(_numChildren);

		for (int i = 0; i < this->_numChildren; i++)
		{
			_childrenList.push_back(std::move(std::make_unique<ModifiedNode_SkinnedMesh>(this)));
			
			//�ڽ� ����� Transform ���� ��ü�� �״�� �������ش�. (�׿��� ����)
			//AnimTransform ������� , �̿� �°� �־��ش�.
			_childrenList.back()->_relTransform->SetParent(this->_relTransform.get(), false);

			//��������� ���.
			this->_childrenList.at(i)->RecursiveInitFromNode(nodeAssetData->_childrenList.at(i).get(), recordMap);
		}

		//RecursiveInitFromNode(nodeAssetData->_childrenList.at(i));
	}

}