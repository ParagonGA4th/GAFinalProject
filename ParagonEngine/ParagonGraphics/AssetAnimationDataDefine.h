#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <DirectXMath.h>
#include <dxtk/SimpleMath.h>

#include "../ParagonData/AnimTransform.h"

/// <summary>
/// �����丵��, ���� �ʿ��� ��ŭ�� Animation�� �����͸� ��� ����
/// ����ü���� �����.
/// ������ ����θ� �ᱹ ��ü�� �뵵�̴�!
/// </summary>

namespace Pg::Graphics
{
	struct Node_AssetData;
	struct Mesh_AssetData;

	//NodeAnim�� Ȱ��, Vector Key (Animation)
	struct VectorKey_AssetData
	{
	public:
		//�ش� Key�� �ߵ� Keyframe.
		double _time;

		//Vector Key�� ��.
		DirectX::SimpleMath::Vector3 _value;

		VectorKey_AssetData() noexcept : _time(0.0), _value() {}
		VectorKey_AssetData(double time, const DirectX::SimpleMath::Vector3& value) :
			_time(time), _value(value) {}

		using elem_type = DirectX::SimpleMath::Vector3;

		// std::find ��� ���� �����ε� (��ġ ����)
		bool operator==(const VectorKey_AssetData& rhs) const
		{
			return rhs._value == this->_value;
		}
		bool operator!=(const VectorKey_AssetData& rhs) const {
			return rhs._value != this->_value;
		}

		// std::sort ��� ���� �����ε� (��� ����)
		bool operator<(const VectorKey_AssetData& rhs) const {
			return _time < rhs._time;
		}

		bool operator>(const VectorKey_AssetData& rhs) const {
			return _time > rhs._time;
		}
	};

	//NodeAnim�� Ȱ��, Quaternion Key (Animation)
	struct QuatKey_AssetData
	{
	public:
		//�ش� Key �ߵ� Keyframe.
		double _time;

		//�ش� Key�� ��.
		DirectX::SimpleMath::Quaternion _value;

		QuatKey_AssetData() noexcept : _time(0.0), _value() {}
		QuatKey_AssetData(double time, const DirectX::SimpleMath::Quaternion& value) :
			_time(time), _value(value) {}

		using elem_type = DirectX::SimpleMath::Quaternion;

		// std::find ��� ���� �����ε� (��ġ ����)
		bool operator==(const QuatKey_AssetData& rhs) const {
			return rhs._value == this->_value;
		}

		bool operator!=(const QuatKey_AssetData& rhs) const {
			return rhs._value != this->_value;
		}

		// std::sort ��� ���� �����ε� (��� ����)
		bool operator<(const QuatKey_AssetData& rhs) const {
			return _time < rhs._time;
		}

		bool operator>(const QuatKey_AssetData& rhs) const {
			return _time > rhs._time;
		}
	};

	struct NodeAnim_AssetData
	{
	public:
		NodeAnim_AssetData();
		~NodeAnim_AssetData();

		//�ڽ��� ���ε� ���� Node�� �̸�.
		std::string _nodeName;

		//Position Key���� ����.
		unsigned int _numPositionKeys = 0;

		//Position Key���� ����Ʈ.
		std::vector<VectorKey_AssetData> _positionKeyList;

		//Rotation Key���� ����.
		unsigned int _numRotationKeys = 0;

		//Rotation Key���� ����Ʈ.
		std::vector<QuatKey_AssetData> _rotationKeyList;

		//Scaling Key���� ����.
		unsigned int _numScalingKeys = 0;

		//Scaling Key���� ����.
		std::vector<VectorKey_AssetData> _scalingKeyList;
	};

	//Animtion�� �����ϱ� ���� ���Ǵ� ����ü.
	struct Animation_AssetData
	{
	public:
		//Animation �̸�.
		std::string _animName;

		//Based Model �̸�.
		std::string _basedModelName;

		//Tick ���� Animation�� ����.
		double _durationTick;

		//�� �� Tick�� ����.
		double _ticksPerSecond;

		//���� �ϳ��� Node�� ������ ��ġ��, NodeAnim Channel�� ����.
		unsigned int _numChannels;

		//���� �ϳ��� Node�� ������ ��ġ��, NodeAnim Channel���� ����Ʈ.
		std::vector<std::unique_ptr<NodeAnim_AssetData>> _channelList;

	};

	//������ ��, Shader�� Bone���� ��� Ȱ�� ����ü.
	struct BoneInfo_AssetData
	{
		BoneInfo_AssetData();
		//FinalTransformation�� Animation ��ü���� �����ϰ� ���� ���̴�.
		//DirectX::SimpleMath::Matrix _finalTransformation; // Final transformation to apply to vertices 
		
		std::string _name;
		UINT _index;
		Node_AssetData* _bindedNode;
		DirectX::SimpleMath::Matrix _offsetMatrix; // Initial offset from local to bone space. 
	};

	class VertexBone_TempAssetData
	{
	public:
		VertexBone_TempAssetData();

		void AddBoneData(unsigned int BoneID, float Weight);
		bool isInit = false;

		//�̰� ���� Vertex Array�� �ݿ����� �������� ���� �ȴ�.
		int idWeightSize = 0;

		unsigned int IDs[4]; //!< An array of 4 bone Ids that influence a single vertex.
		float Weights[4]; //!< An array of the weight influence per bone. 
	};

	//���� Node�ʹ� �޸�, ���� �����Ϳ� ������ ���� SkinnedMesh3D�� �ν��Ͻ��� ������ �� �ְ� �ϱ� ����.
	struct ModifiedNode_SkinnedMesh
	{
	public:
		ModifiedNode_SkinnedMesh(ModifiedNode_SkinnedMesh* parentNode);
		~ModifiedNode_SkinnedMesh();

		void RecursiveInitFromNode(Node_AssetData* nodeAssetData, std::unordered_map<std::string, const ModifiedNode_SkinnedMesh*>& recordMap);

		UINT _index{ 0 };
		std::string _nodeName;
		const Node_AssetData* _originData;
		std::unique_ptr<Pg::Data::AnimTransform> _relTransform; //-> �̴� NodeHierarchy�� ������ ���纻���� ������� ���̴�.
		ModifiedNode_SkinnedMesh* _parentNode;
		unsigned int _numChildren{ 0 }; //�ش� Node�� Children ����.
		std::vector<std::unique_ptr<ModifiedNode_SkinnedMesh>> _childrenList; //�� Node�� Children Node��. (�ڽ� ��� ������ nullptr)
		unsigned int _numMeshes{ 0 }; //�ش� Node�� Mesh ����.
		std::vector<const Mesh_AssetData*> _meshList; //Mesh Index ����. (���� aiScene�� MeshList�� ����)

		//���� ���ε��� Bone�� ������, ������ ����.
		BoneInfo_AssetData* _bindedBone{ nullptr };
	};

}
