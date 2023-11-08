#pragma once
#include <DirectXMath.h>
#include <dxtk/SimpleMath.h>

/// 일단 단순 Skinning 구현을 위해 설정해놓는 헬퍼 구조체들.
namespace Pg::Graphics
{
	struct RenderUsageMesh
	{
		unsigned int _baseVertex = 0;  //!< The base vertex of this mesh in the vertices array for the entire model.
		unsigned int _baseIndex = 0;	  //!< The base index of this mesh in the indices array for the entire model. 
		unsigned int _numVertices = 0; // 해당 Mesh에 종속되는 Vertex 개수.
		unsigned int _numIndices = 0;  //!< Total number of mesh indices. 
	};

	class RenderUsageVertexBone
	{
	public:
		RenderUsageVertexBone();
	
		void AddBoneData(unsigned int BoneID, float Weight);
		bool isInit = false;

		//이걸 보고 Vertex Array를 반영할지 안할지를 보면 된다.
		int idWeightSize = 0;

		unsigned int IDs[4]; //!< An array of 4 bone Ids that influence a single vertex.
		float Weights[4]; //!< An array of the weight influence per bone. 
	};

	class RenderUsageBoneInfo
	{
	public:
		RenderUsageBoneInfo();
		DirectX::SimpleMath::Matrix _finalTransformation; // Final transformation to apply to vertices 
		DirectX::SimpleMath::Matrix _boneOffset; // Initial offset from local to bone space. 
	};

}
