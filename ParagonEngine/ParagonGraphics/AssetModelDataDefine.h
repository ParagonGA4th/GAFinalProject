#pragma once
#include "RenderPrepStructs.h"
#include "AssetAnimationDataDefine.h"
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <DirectXMath.h>
#include <dxtk/SimpleMath.h>

/// <summary>
/// 리팩토링된, 새로 필요한 만큼만 3DModel의 데이터를 들고 있을
/// 구조체들의 선언부.
/// 기존의 선언부를 결국 대체할 용도이다!
/// </summary>
/// 
namespace Pg::Graphics
{
	class RenderAnimation;
	class Mesh_AssetData;
}

namespace Pg::Graphics
{
	// 개별적으로 Static Mesh 기준으로 -> 이를 쓰지 않음.
	// Skinned Mesh는 Rendering을 위해 써야 함!
	struct Node_AssetData
	{
		Node_AssetData(Node_AssetData* parentNode);
		~Node_AssetData();

		UINT _index;
		std::string _nodeName;
		DirectX::XMMATRIX _offsetMatrix; 
		//std::unique_ptr<Pg::Data::Transform> _relTransform; -> 이는 NodeHierarchy를 따라한 복사본에서 만들어질 것이다.
		Node_AssetData* _parentNode = nullptr;
		unsigned int _numChildren; //해당 Node의 Children 개수.
		std::vector<std::unique_ptr<Node_AssetData>> _childrenList; //이 Node의 Children Node들. (자식 노드 없으면 nullptr)
		unsigned int _numMeshes; //해당 Node의 Mesh 개수.
		std::vector<const Mesh_AssetData*> _meshList; //Node에 소속될 Mesh들 저장. (각자 aiScene의 MeshList에 대응)
	
		//만약 바인딩된 Bone이 있으면, 포인터 보관.
		BoneInfo_AssetData* _bindedBone{ nullptr };
	};

	//Skinned Renderer 도입시 쓰일 예정.
	//struct Bone_AssetData
	//{
	//	Bone_AssetData();
	//
	//	std::string _name; //Bone 이름, NodeName과 1대1 매칭될 것.
	//	unsigned int _numWeights; // 이 Bone에 의해 영향 받는 Vertex 개수.
	//	DirectX::SimpleMath::Matrix _offsetMatrix; //Mesh Space -> Bone Space로 바꿔주는 Inverse Bind Pose Matrix.
	//};

	//각 Mesh당 저장될 AABB 데이터.
	//struct AABB_AssetData
	//{
	//	DirectX::SimpleMath::Vector3 _minVec;
	//	DirectX::SimpleMath::Vector3 _maxVec;
	//};

	//렌더에 필요한 Mesh의 정보만 보관 및, 렌더에 활용될 것. 
	struct Mesh_AssetData
	{
		//자기 자신이 속한 노드 인덱스.
		UINT _belongNodeIndex = 0;

		//Vertex Starting Points Per Mesh (이때부터 자신을 Draw) == BaseVertex
		unsigned int _vertexOffset = 0;

		//Index Starting Points Per Mesh (이때부터 자신을 Draw) == BaseIndex
		unsigned int _indexOffset = 0;

		//해당 Mesh 내부의 Vertex 개수.
		unsigned int _numVertices = 0; 

		//해당 Mesh 내부의 Indice 개수.
		unsigned int _numIndices = 0; 

		//자신이 속한 MaterialID
		unsigned int _materialID = 0;

		DirectX::BoundingBox _AABB;
	};

	struct Scene_AssetData
	{
		std::string _directory; //Material Load를 위해 보관.
		std::unique_ptr<Node_AssetData> _rootNode = nullptr;

		//렌더에 필요한 Mesh의 정보 한정 보관.
		std::vector<Mesh_AssetData> _meshList;

		//전체 Mesh의 개수.
		unsigned int _totalMeshCount = 0;

		//전체 Vertex Count.
		unsigned int _totalVertexCount = 0;

		//전체 Index Count.
		unsigned int _totalIndexCount = 0;

		//전체 Material Count;
		unsigned int _totalMaterialCount = 0;

		//Vertex Buffer와 일대일 대응하는 Position 기록.
		std::vector<DirectX::XMFLOAT3> _posRecordVector;
		//Vertex Buffer랑 1대1 대응하는 Texture0 (Float2) 기록.
		std::vector<DirectX::XMFLOAT2> _texRecordVector;
		//Vertex Buffer랑 1대1 대응하는 MeshMatID 기록.
		std::vector<float> _meshMatIDRecordVector;
	};

	//SkinnedMeshRenderer에 활용되는 데이터들, 해당 구조체에 모아서 관리.
	struct Skinned_AssetData
	{
		Skinned_AssetData();

		//Bone 연산에 필요한 Global Inverse Transform.
		DirectX::SimpleMath::Matrix _meshGlobalInverseTransform;

		//개별적인 인스턴스로 이동.
		//명시적으로 Node들의 List를 가지고 있음 -> String 값을 통해 Node들을 찾을 수 있음.
		//std::unordered_map<std::string, const Node_AssetData*> _animatedNodeMap;

		//RenderAnimation 자체가 자료에 접근해서, 호환되는 자신을 추가한다.
		std::map<std::string, RenderAnimation*> _viableAnimations;

		struct BlendDataRecord
		{
			unsigned int	  _blendIndice0;
			unsigned int	  _blendIndice1;
			unsigned int	  _blendIndice2;
			unsigned int	  _blendIndice3;

			float			  _blendWeight0;
			float			  _blendWeight1;
			float			  _blendWeight2;
		};

		//posRecordVector와 동일한 인덱스에 BlendIndice 등 정보가 등록된다.
		std::vector<BlendDataRecord> _blendDataRecordVector;
	};
}