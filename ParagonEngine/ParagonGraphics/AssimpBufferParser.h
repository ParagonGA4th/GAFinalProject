#pragma once
#include "AssetAnimationDataDefine.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

/// <summary>
/// AssetScene�� ��ǲ���̾ƿ��� ������� VB�� IB�� ������ִ� Ŭ����.
/// �Ŀ� ���ҽ��� D3D�� �����ϴ� ���ҵ� ������ ����.
/// Assimp �����͸� �Ľ��ϴ� ���ҵ� ����!
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
	//���� ��ü Mesh ���� ����
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
	class Asset3DModelData;

	//Material Cluster.
	class MaterialCluster;
	class RenderTexture2DArray;
}

namespace Pg::Util
{
	class InstancingException;
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

		static void Initialize();

		//Assimp�� ����ü�� �ް� Vertex / Index Buffer�� �����.
		//�ϴ��� 1�� �׷��ȿ������� Screen->World Space 1�� Layout�� ���ߵ��� ����.
		static void AssimpToStaticDataDXBuffer(const aiScene* assimp, Scene_AssetData* sceneData, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB);
		static void AssimpToSkinnedDataDXBuffer(const aiScene* assimp, Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB);
		static void AssimpToSceneAssetData(const aiScene* assimp, const std::string& path, Scene_AssetData* outSceneAssetData);
		static void AssimpToMaterialClusterList(const aiScene* assimp, bool& bCheckIfUseAlphaBlending, bool& bChecKIfUseAlphaClipping, std::vector<MaterialCluster*>& outMatClusterList, const std::string& directory);
		static void AssimpToPBRTextureArray(const std::string& modelName, const std::string& modelPath, bool isSkinned, std::vector<MaterialCluster*>& outMatClusterList, RenderTexture2DArray** outArrayData);
		static void D3DSetPrivateData(const std::string& modelName, Asset3DModelData* modelData);
		//�� �����Ӹ��� �� ����, Reset()�� ���ؼ� ��������� AssimpBufferParser�� �����ϴٴ� ����� �巯���� �Ѵ�.
		static void Reset();
		//�Ŀ�, ���⿡ Animation�� �ε��ϴ� �Լ��� ���� �� ��.
	
	private:
		//���������� VB/IB�� ���������� �Լ���.
		static void ParseAssimpStatic(const aiScene* assimp, Scene_AssetData* sceneData, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt);
		static void ParseAssimpSkinned(const aiScene* assimp, Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, const std::vector<VertexBone_TempAssetData>& vertexBoneVector, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt);

		//Skinned Data �����ϴ� �Լ�.
		static void StoreIndependentSkinnedData(const aiScene* assimp, Skinned_AssetData* skinnedData);

		//���������� VB ���� �� ���̴� VertexBone ���� ��ȯ.
		static void StoreGetDependentSkinnedData(const aiScene* assimp, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<VertexBone_TempAssetData>& outVertexBoneVector);
		static void SetupRenderBones(unsigned int index, aiMesh* mesh, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<VertexBone_TempAssetData>& vBoneList);

		//�������� Assimp ����ü�� AssetData�� �Űܼ� �����Ѵ�.
		static void StoreAssimpNode(const aiNode* assimp, Scene_AssetData* sceneData, Node_AssetData* pgNode, UINT& index);
		
		//��ü������ Mesh_AssetData�� ���θ� ���������
		static void StoreAssimpMesh(const aiMesh* assimp, Mesh_AssetData* pgMesh, unsigned int vOffset, unsigned int iOffset);
		static void StoreAssimpAABB(const aiAABB* assimp, DirectX::BoundingBox* pgAABB);
		//static void StoreAssimpBone(const aiBone* assimp, Bone_AssetData* pgAABB); 
		//Skinned �� ��.

		static void StoreAssimpVectorKey(const aiVectorKey* assimp, VectorKey_AssetData* pgVectorKey);
		static void StoreAssimpQuatKey(const aiQuatKey* assimp, QuatKey_AssetData* pgQuatKey);
		static void StoreAssimpNodeAnim(const aiNodeAnim* assimp, NodeAnim_AssetData* pgNodeAnim);
		static void StoreAssimpAnimation(const aiAnimation* assimp, Animation_AssetData* pgAnim);
		//Bone Info�� �־�� �ϴµ�..

		//��� ��Ͽ�����.
		 
		//Material ����.
		//static void ConnectMeshToNode();

	private:
		static std::unordered_map<const aiNode*, Node_AssetData*> _aiNodeToNodeMap;	// Bone���� aiNode�� ���� Node�� ã������ ��
		static std::unordered_map<const aiMesh*, Mesh_AssetData*> _aiMeshToMeshMap; // aiMesh�� ���� Mesh�� ã�� ���� ��.
	
	private:
		static Pg::Util::InstancingException* _instancingException;
	};
}


