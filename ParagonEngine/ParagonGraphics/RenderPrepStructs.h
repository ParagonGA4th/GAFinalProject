#pragma once
#include <DirectXMath.h>
#include <dxtk/SimpleMath.h>

/// �ϴ� �ܼ� Skinning ������ ���� �����س��� ���� ����ü��.
namespace Pg::Graphics
{
	struct RenderPrepMesh
	{
		unsigned int _baseVertex = 0;  //!< The base vertex of this mesh in the vertices array for the entire model.
		unsigned int _baseIndex = 0;	  //!< The base index of this mesh in the indices array for the entire model. 
		unsigned int _numVertices = 0; // �ش� Mesh�� ���ӵǴ� Vertex ����.
		unsigned int _numIndices = 0;  //!< Total number of mesh indices. 
	};

	class RenderPrepVertexBone
	{
	public:
		RenderPrepVertexBone();

		void AddBoneData(unsigned int BoneID, float Weight);
		bool isInit = false;

		//�̰� ���� Vertex Array�� �ݿ����� �������� ���� �ȴ�.
		int idWeightSize = 0;

		unsigned int IDs[4]; //!< An array of 4 bone Ids that influence a single vertex.
		float Weights[4]; //!< An array of the weight influence per bone. 
	};

	class RenderPrepBoneInfo
	{
	public:
		RenderPrepBoneInfo();
		DirectX::SimpleMath::Matrix _finalTransformation; // Final transformation to apply to vertices 
		DirectX::SimpleMath::Matrix _boneOffset; // Initial offset from local to bone space. 
	};

	////���� Tofu���� ���̴� Mesh ���.
	struct Mesh
	{
		float			matrix[12];
		unsigned int	startVertex;
		unsigned int	startIndex;
		unsigned int	numVertices;
		unsigned int	numIndices;
	};
}
