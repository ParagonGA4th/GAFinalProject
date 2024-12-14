#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <DirectXMath.h>
#include <dxtk/SimpleMath.h>

#include "../ParagonData/AnimTransform.h"

/// <summary>
/// 리팩토링된, 새로 필요한 만큼만 Animation의 데이터를 들고 있을
/// 구조체들의 선언부.
/// 기존의 선언부를 결국 대체할 용도이다!
/// </summary>

namespace Pg::Graphics
{
	struct Node_AssetData;
	struct Mesh_AssetData;

	//NodeAnim에 활용, Vector Key (Animation)
	struct VectorKey_AssetData
	{
	public:
		//해당 Key의 발동 Keyframe.
		double _time;

		//Vector Key의 값.
		DirectX::SimpleMath::Vector3 _value;

		VectorKey_AssetData() noexcept : _time(0.0), _value() {}
		VectorKey_AssetData(double time, const DirectX::SimpleMath::Vector3& value) :
			_time(time), _value(value) {}

		using elem_type = DirectX::SimpleMath::Vector3;

		// std::find 사용 가능 오버로드 (일치 여부)
		bool operator==(const VectorKey_AssetData& rhs) const
		{
			return rhs._value == this->_value;
		}
		bool operator!=(const VectorKey_AssetData& rhs) const {
			return rhs._value != this->_value;
		}

		// std::sort 사용 가능 오버로드 (대소 관계)
		bool operator<(const VectorKey_AssetData& rhs) const {
			return _time < rhs._time;
		}

		bool operator>(const VectorKey_AssetData& rhs) const {
			return _time > rhs._time;
		}
	};

	//NodeAnim에 활용, Quaternion Key (Animation)
	struct QuatKey_AssetData
	{
	public:
		//해당 Key 발동 Keyframe.
		double _time;

		//해당 Key의 값.
		DirectX::SimpleMath::Quaternion _value;

		QuatKey_AssetData() noexcept : _time(0.0), _value() {}
		QuatKey_AssetData(double time, const DirectX::SimpleMath::Quaternion& value) :
			_time(time), _value(value) {}

		using elem_type = DirectX::SimpleMath::Quaternion;

		// std::find 사용 가능 오버로드 (일치 여부)
		bool operator==(const QuatKey_AssetData& rhs) const {
			return rhs._value == this->_value;
		}

		bool operator!=(const QuatKey_AssetData& rhs) const {
			return rhs._value != this->_value;
		}

		// std::sort 사용 가능 오버로드 (대소 관계)
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

		//자신이 매핑될 실제 Node의 이름.
		std::string _nodeName;

		//Position Key들의 개수.
		unsigned int _numPositionKeys = 0;

		//Position Key들의 리스트.
		std::vector<VectorKey_AssetData> _positionKeyList;

		//Rotation Key들의 개수.
		unsigned int _numRotationKeys = 0;

		//Rotation Key들의 리스트.
		std::vector<QuatKey_AssetData> _rotationKeyList;

		//Scaling Key들의 개수.
		unsigned int _numScalingKeys = 0;

		//Scaling Key들의 개수.
		std::vector<VectorKey_AssetData> _scalingKeyList;
	};

	//Animtion을 보관하기 위해 사용되는 구조체.
	struct Animation_AssetData
	{
	public:
		//Animation 이름.
		std::string _animName;

		//Based Model 이름.
		std::string _basedModelName;

		//Tick 기준 Animation의 길이.
		double _durationTick;

		//초 당 Tick의 개수.
		double _ticksPerSecond;

		//각각 하나의 Node에 영향을 미치는, NodeAnim Channel의 개수.
		unsigned int _numChannels;

		//각각 하나의 Node에 영향을 미치는, NodeAnim Channel들의 리스트.
		std::vector<std::unique_ptr<NodeAnim_AssetData>> _channelList;

	};

	//렌더될 때, Shader에 Bone들의 목록 활용 구조체.
	struct BoneInfo_AssetData
	{
		BoneInfo_AssetData();
		//FinalTransformation은 Animation 자체에서 관리하고 있을 것이다.
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

		//이걸 보고 Vertex Array를 반영할지 안할지를 보면 된다.
		int idWeightSize = 0;

		unsigned int IDs[4]; //!< An array of 4 bone Ids that influence a single vertex.
		float Weights[4]; //!< An array of the weight influence per bone. 
	};

	//기존 Node와는 달리, 원본 데이터와 별도로 개별 SkinnedMesh3D의 인스턴스를 제어할 수 있게 하기 위해.
	struct ModifiedNode_SkinnedMesh
	{
	public:
		ModifiedNode_SkinnedMesh(ModifiedNode_SkinnedMesh* parentNode);
		~ModifiedNode_SkinnedMesh();

		void RecursiveInitFromNode(Node_AssetData* nodeAssetData, std::unordered_map<std::string, const ModifiedNode_SkinnedMesh*>& recordMap);

		UINT _index{ 0 };
		std::string _nodeName;
		const Node_AssetData* _originData;
		std::unique_ptr<Pg::Data::AnimTransform> _relTransform; //-> 이는 NodeHierarchy를 따라한 복사본에서 만들어질 것이다.
		ModifiedNode_SkinnedMesh* _parentNode;
		unsigned int _numChildren{ 0 }; //해당 Node의 Children 개수.
		std::vector<std::unique_ptr<ModifiedNode_SkinnedMesh>> _childrenList; //이 Node의 Children Node들. (자식 노드 없으면 nullptr)
		unsigned int _numMeshes{ 0 }; //해당 Node의 Mesh 개수.
		std::vector<const Mesh_AssetData*> _meshList; //Mesh Index 저장. (각자 aiScene의 MeshList에 대응)

		//만약 바인딩된 Bone이 있으면, 포인터 보관.
		BoneInfo_AssetData* _bindedBone{ nullptr };
	};

}
