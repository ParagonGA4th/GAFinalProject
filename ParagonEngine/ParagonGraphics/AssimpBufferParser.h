#pragma once
#include "RenderPrepStructs.h"

#include <string>
#include <vector>
#include <memory>

/// <summary>
/// AssetScene을 인풋레이아웃을 기반으로 VB와 IB로 만들어주는 클래스.
/// 후에 리소스를 D3D와 연동하는 역할도 수행할 예정.
/// Assimp 데이터를 파싱하는 역할도 수행!
/// </summary>

//Assimp
struct aiScene;
struct aiNode;
struct aiMesh;
struct aiBone;
struct aiTexture;
struct aiVertexWeight;
struct aiAABB;
struct aiVectorKey;
struct aiQuatKey;
struct aiNodeAnim;
struct aiAnimation;
struct aiMaterial;

//DirectX 
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

namespace Pg::Graphics
{
	//엔진 자체 Mesh 정보 저장
	struct Scene_AssetData;
	struct Skinned_AssetData;
	class Node_AssetData;
	class Mesh_AssetData;
	class AABB_AssetData;
	class Bone_AssetData;

	class VectorKey_AssetData;
	class QuatKey_AssetData;
	class NodeAnim_AssetData;
	class Animation_AssetData;
	class BoneInfo_AssetData;

	//Material Cluster.
	class MaterialCluster;
	class RenderTexture2DArray;
}

namespace Pg::Graphics
{
	namespace Manager
	{
		class GraphicsResourceManager;
	}
}

namespace Pg::Graphics::Helper
{
	class AssimpBufferParser
	{
	public:
		AssimpBufferParser();
		~AssimpBufferParser();

		//Assimp의 구조체를 받고 Vertex / Index Buffer를 만든다.
		//일단은 1차 그래픽엔진에서 Screen->World Space 1차 Layout에 맞추도록 세팅.
		static void AssimpToStaticDataDXBuffer(const aiScene* assimp, Scene_AssetData* sceneData, ID3D11Buffer*& outVB, ID3D11Buffer*& outIB);
		static void AssimpToSkinnedDataDXBuffer(const aiScene* assimp, Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, ID3D11Buffer*& outVB, ID3D11Buffer*& outIB);
		static void AssimpToSceneAssetData(const aiScene* assimp, const std::string& path, Scene_AssetData* outSceneAssetData);
		static void AssimpToMaterialClusterList(const aiScene* assimp, std::vector<MaterialCluster*>& outMatClusterList, const std::string& directory);
		static void AssimpToPBRTextureArray(std::vector<MaterialCluster*>& outMatClusterList, RenderTexture2DArray** outArrayData);

		//후에, 여기에 Animation을 로드하는 함수가 들어가야 할 것.
	
	private:
		//직접적으로 VB/IB를 만들어내보내는 함수들.
		static void ParseAssimpStatic(const aiScene* assimp, Scene_AssetData* sceneData, ID3D11Buffer*& outVB, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt);
		static void ParseAssimpSkinned(const aiScene* assimp, Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, const std::vector<RenderPrepVertexBone>& vertexBoneVector, ID3D11Buffer*& outVB, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt);

		//Skinned Data 저장하는 함수.
		static void StoreIndependentSkinnedData(const aiScene* assimp, Skinned_AssetData* skinnedData);

		//참조형으로 VB 만들 때 쓰이는 VertexBone 벡터 반환.
		static void StoreGetDependentSkinnedData(const aiScene* assimp, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<RenderPrepVertexBone>& outVertexBoneVector);
		static void SetupRenderBones(unsigned int index, aiMesh* mesh, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<RenderPrepVertexBone>& vBoneList);

		//개별적인 Assimp 구조체를 AssetData로 옮겨서 저장한다.
		static void StoreAssimpNode(const aiNode* assimp, Node_AssetData* pgNode);
		static void StoreAssimpMesh(const aiMesh* assimp, Mesh_AssetData* pgMesh, unsigned int vOffset, unsigned int iOffset);
		static void StoreAssimpAABB(const aiAABB* assimp, AABB_AssetData* pgAABB);
		//static void StoreAssimpBone(const aiBone* assimp, Bone_AssetData* pgAABB); 
		//Skinned 할 때.

		static void StoreAssimpVectorKey(const aiVectorKey* assimp, VectorKey_AssetData* pgVectorKey);
		static void StoreAssimpQuatKey(const aiQuatKey* assimp, QuatKey_AssetData* pgQuatKey);
		static void StoreAssimpNodeAnim(const aiNodeAnim* assimp, NodeAnim_AssetData* pgNodeAnim);
		static void StoreAssimpAnimation(const aiAnimation* assimp, Animation_AssetData* pgAnim);
		//Bone Info도 있어야 하는데..


		 
		//Material 관련.

	private:
		//Skinned 렌더위한 값을 가져오기 위해서.


	};
}


