#pragma once
#include "AssetAnimationDataDefine.h"

#include <string>
#include <unordered_map>
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

		//Assimp의 구조체를 받고 Vertex / Index Buffer를 만든다.
		//일단은 1차 그래픽엔진에서 Screen->World Space 1차 Layout에 맞추도록 세팅.
		static void AssimpToStaticDataDXBuffer(const aiScene* assimp, Scene_AssetData* sceneData, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB);
		static void AssimpToSkinnedDataDXBuffer(const aiScene* assimp, Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB);
		static void AssimpToSceneAssetData(const aiScene* assimp, const std::string& path, Scene_AssetData* outSceneAssetData);
		static void AssimpToMaterialClusterList(const aiScene* assimp, bool& bCheckIfUseAlphaBlending, bool& bChecKIfUseAlphaClipping, std::vector<MaterialCluster*>& outMatClusterList, const std::string& directory);
		static void AssimpToPBRTextureArray(const std::string& modelName, const std::string& modelPath, bool isSkinned, std::vector<MaterialCluster*>& outMatClusterList, RenderTexture2DArray** outArrayData);
		static void D3DSetPrivateData(const std::string& modelName, Asset3DModelData* modelData);
		//매 프레임마다 값 관리, Reset()을 통해서 명시적으로 AssimpBufferParser가 재사용하다는 사실을 드러내야 한다.
		static void Reset();
		//후에, 여기에 Animation을 로드하는 함수가 들어가야 할 것.
	
	private:
		//직접적으로 VB/IB를 만들어내보내는 함수들.
		static void ParseAssimpStatic(const aiScene* assimp, Scene_AssetData* sceneData, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt);
		static void ParseAssimpSkinned(const aiScene* assimp, Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, const std::vector<VertexBone_TempAssetData>& vertexBoneVector, ID3D11Buffer*& outVB1st, ID3D11Buffer*& outVB2nd, ID3D11Buffer*& outIB, unsigned int vertexCnt, unsigned int indexCnt);

		//Skinned Data 저장하는 함수.
		static void StoreIndependentSkinnedData(const aiScene* assimp, Skinned_AssetData* skinnedData);

		//참조형으로 VB 만들 때 쓰이는 VertexBone 벡터 반환.
		static void StoreGetDependentSkinnedData(const aiScene* assimp, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<VertexBone_TempAssetData>& outVertexBoneVector);
		static void SetupRenderBones(unsigned int index, aiMesh* mesh, const Scene_AssetData* sceneData, Skinned_AssetData* skinnedData, std::vector<VertexBone_TempAssetData>& vBoneList);

		//개별적인 Assimp 구조체를 AssetData로 옮겨서 저장한다.
		static void StoreAssimpNode(const aiNode* assimp, Scene_AssetData* sceneData, Node_AssetData* pgNode, UINT& index);
		
		//자체에서는 Mesh_AssetData의 전부만 기록하지는
		static void StoreAssimpMesh(const aiMesh* assimp, Mesh_AssetData* pgMesh, unsigned int vOffset, unsigned int iOffset);
		static void StoreAssimpAABB(const aiAABB* assimp, DirectX::BoundingBox* pgAABB);
		//static void StoreAssimpBone(const aiBone* assimp, Bone_AssetData* pgAABB); 
		//Skinned 할 때.

		static void StoreAssimpVectorKey(const aiVectorKey* assimp, VectorKey_AssetData* pgVectorKey);
		static void StoreAssimpQuatKey(const aiQuatKey* assimp, QuatKey_AssetData* pgQuatKey);
		static void StoreAssimpNodeAnim(const aiNodeAnim* assimp, NodeAnim_AssetData* pgNodeAnim);
		static void StoreAssimpAnimation(const aiAnimation* assimp, Animation_AssetData* pgAnim);
		//Bone Info도 있어야 하는데..

		//노드 기록용으로.
		 
		//Material 관련.
		//static void ConnectMeshToNode();

	private:
		static std::unordered_map<const aiNode*, Node_AssetData*> _aiNodeToNodeMap;	// Bone에서 aiNode를 통해 Node를 찾기위한 맵
		static std::unordered_map<const aiMesh*, Mesh_AssetData*> _aiMeshToMeshMap; // aiMesh를 통해 Mesh를 찾기 위한 맵.
	
	private:
		static Pg::Util::InstancingException* _instancingException;
	};
}


