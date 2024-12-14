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
/// �����丵��, ���� �ʿ��� ��ŭ�� 3DModel�� �����͸� ��� ����
/// ����ü���� �����.
/// ������ ����θ� �ᱹ ��ü�� �뵵�̴�!
/// </summary>
/// 
namespace Pg::Graphics
{
	class RenderAnimation;
	class Mesh_AssetData;
}

namespace Pg::Graphics
{
	// ���������� Static Mesh �������� -> �̸� ���� ����.
	// Skinned Mesh�� Rendering�� ���� ��� ��!
	struct Node_AssetData
	{
		Node_AssetData(Node_AssetData* parentNode);
		~Node_AssetData();

		UINT _index;
		std::string _nodeName;
		DirectX::XMMATRIX _offsetMatrix; 
		//std::unique_ptr<Pg::Data::Transform> _relTransform; -> �̴� NodeHierarchy�� ������ ���纻���� ������� ���̴�.
		Node_AssetData* _parentNode = nullptr;
		unsigned int _numChildren; //�ش� Node�� Children ����.
		std::vector<std::unique_ptr<Node_AssetData>> _childrenList; //�� Node�� Children Node��. (�ڽ� ��� ������ nullptr)
		unsigned int _numMeshes; //�ش� Node�� Mesh ����.
		std::vector<const Mesh_AssetData*> _meshList; //Node�� �Ҽӵ� Mesh�� ����. (���� aiScene�� MeshList�� ����)
	
		//���� ���ε��� Bone�� ������, ������ ����.
		BoneInfo_AssetData* _bindedBone{ nullptr };
	};

	//Skinned Renderer ���Խ� ���� ����.
	//struct Bone_AssetData
	//{
	//	Bone_AssetData();
	//
	//	std::string _name; //Bone �̸�, NodeName�� 1��1 ��Ī�� ��.
	//	unsigned int _numWeights; // �� Bone�� ���� ���� �޴� Vertex ����.
	//	DirectX::SimpleMath::Matrix _offsetMatrix; //Mesh Space -> Bone Space�� �ٲ��ִ� Inverse Bind Pose Matrix.
	//};

	//�� Mesh�� ����� AABB ������.
	//struct AABB_AssetData
	//{
	//	DirectX::SimpleMath::Vector3 _minVec;
	//	DirectX::SimpleMath::Vector3 _maxVec;
	//};

	//������ �ʿ��� Mesh�� ������ ���� ��, ������ Ȱ��� ��. 
	struct Mesh_AssetData
	{
		//�ڱ� �ڽ��� ���� ��� �ε���.
		UINT _belongNodeIndex = 0;

		//Vertex Starting Points Per Mesh (�̶����� �ڽ��� Draw) == BaseVertex
		unsigned int _vertexOffset = 0;

		//Index Starting Points Per Mesh (�̶����� �ڽ��� Draw) == BaseIndex
		unsigned int _indexOffset = 0;

		//�ش� Mesh ������ Vertex ����.
		unsigned int _numVertices = 0; 

		//�ش� Mesh ������ Indice ����.
		unsigned int _numIndices = 0; 

		//�ڽ��� ���� MaterialID
		unsigned int _materialID = 0;

		DirectX::BoundingBox _AABB;
	};

	struct Scene_AssetData
	{
		std::string _directory; //Material Load�� ���� ����.
		std::unique_ptr<Node_AssetData> _rootNode = nullptr;

		//������ �ʿ��� Mesh�� ���� ���� ����.
		std::vector<Mesh_AssetData> _meshList;

		//��ü Mesh�� ����.
		unsigned int _totalMeshCount = 0;

		//��ü Vertex Count.
		unsigned int _totalVertexCount = 0;

		//��ü Index Count.
		unsigned int _totalIndexCount = 0;

		//��ü Material Count;
		unsigned int _totalMaterialCount = 0;

		//Vertex Buffer�� �ϴ��� �����ϴ� Position ���.
		std::vector<DirectX::XMFLOAT3> _posRecordVector;
		//Vertex Buffer�� 1��1 �����ϴ� Texture0 (Float2) ���.
		std::vector<DirectX::XMFLOAT2> _texRecordVector;
		//Vertex Buffer�� 1��1 �����ϴ� MeshMatID ���.
		std::vector<float> _meshMatIDRecordVector;
	};

	//SkinnedMeshRenderer�� Ȱ��Ǵ� �����͵�, �ش� ����ü�� ��Ƽ� ����.
	struct Skinned_AssetData
	{
		Skinned_AssetData();

		//Bone ���꿡 �ʿ��� Global Inverse Transform.
		DirectX::SimpleMath::Matrix _meshGlobalInverseTransform;

		//�������� �ν��Ͻ��� �̵�.
		//��������� Node���� List�� ������ ���� -> String ���� ���� Node���� ã�� �� ����.
		//std::unordered_map<std::string, const Node_AssetData*> _animatedNodeMap;

		//RenderAnimation ��ü�� �ڷῡ �����ؼ�, ȣȯ�Ǵ� �ڽ��� �߰��Ѵ�.
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

		//posRecordVector�� ������ �ε����� BlendIndice �� ������ ��ϵȴ�.
		std::vector<BlendDataRecord> _blendDataRecordVector;
	};
}