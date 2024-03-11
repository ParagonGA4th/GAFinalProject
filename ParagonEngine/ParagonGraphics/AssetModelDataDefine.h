#pragma once
#include "RenderPrepStructs.h"
#include "AssetAnimationDataDefine.h"
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <DirectXMath.h>
#include <dxtk/SimpleMath.h>

/// <summary>
/// 리팩토링된, 새로 필요한 만큼만 3DModel의 데이터를 들고 있을
/// 구조체들의 선언부.
/// 기존의 선언부를 결국 대체할 용도이다!
/// </summary>

namespace Pg::Graphics
{
	class RenderAnimation;
}

namespace Pg::Graphics
{
	// 개별적으로 Static Mesh 기준으로 -> 이를 쓰지 않음.
	// Skinned Mesh는 Rendering을 위해 써야 함!
	struct Node_AssetData
	{
		Node_AssetData(Node_AssetData* parentNode);
		~Node_AssetData();

		std::string _nodeName;
		DirectX::SimpleMath::Matrix _relTransform;
		Node_AssetData* _parentNode = nullptr;
		unsigned int _numChildren; //해당 Node의 Children 개수.
		std::vector<std::unique_ptr<Node_AssetData>> _childrenList; //이 Node의 Children Node들. (자식 노드 없으면 nullptr)
		unsigned int _numMeshes; //해당 Node의 Mesh 개수.
		std::vector<unsigned int> _meshIndexList; //Mesh Index 저장. (각자 aiScene의 MeshList에 대응)
	};

	//Skinned Renderer 도입시 쓰일 예정.
	struct Bone_AssetData
	{
		Bone_AssetData();

		std::string _name; //Bone 이름, NodeName과 1대1 매칭될 것.
		unsigned int _numWeights; // 이 Bone에 의해 영향 받는 Vertex 개수.
		DirectX::SimpleMath::Matrix _offsetMatrix; //Mesh Space -> Bone Space로 바꿔주는 Inverse Bind Pose Matrix.
	};

	//각 Mesh당 저장될 AABB 데이터.
	struct AABB_AssetData
	{
		DirectX::SimpleMath::Vector3 _minVec;
		DirectX::SimpleMath::Vector3 _maxVec;
	};

	//렌더에 필요한 Mesh의 정보만 보관 및, 렌더에 활용될 것. 
	struct Mesh_AssetData
	{
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

		AABB_AssetData _AABB;
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

		//매핑될 Bone Info 벡터, 포인터가 RenderAnimation 쪽에서도 계산의 편의를 위해 저장됨.
		std::vector<BoneInfo_AssetData> _renderBoneInfoVector;

		//Bone Mapping 정보 저장.
		std::map<std::string, unsigned int> _mappedBones;
		unsigned int _numFormationBone;

		//재귀식으로 나열된 Node들의 리스트를 Linear하게 나열. 렌더될 때 활용될 것이다.
		//이에 호환되는 NodeAnim들의 리스트는 RenderAnimation에 저장.
		//(NodeAnim이 결부 안된 노드들은, 해당 인덱스를 nullptr로 RenderAnimation에 저장.)
		//SceneData에 호환되는 데이터의 RawPointer들로 나열되어 있다.
		//이론상, 순서대로 돌리면 재귀와 같은 값이 나와야 한다.
		std::vector<const Node_AssetData* const> _linearizedNodeHierarchy;

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