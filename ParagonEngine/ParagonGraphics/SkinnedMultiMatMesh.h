#pragma once
#include "ConstantBufferDefine.h"
#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <array>
#include <DirectXMath.h>
#include <d3d11.h>

//Temporary
#include "TofuMesh.h"
#include "Bone.h"

//임시 기능 위주 Skinning 구현을 위해, 해당 헤더 사용!
#include "TempBoneHelper.h"

#include <assimp/quaternion.h>
#include <assimp/vector3.h>

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiAnimation;
struct aiNodeAnim;

/// <summary>
/// 일단 Single Mesh 전용.
/// </summary>

namespace Assimp
{
	class Importer;
}

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	}
}
namespace Pg::Data
{
	struct CameraData;
}

namespace Pg::Graphics
{
	class Asset3DModelData;
}


namespace Pg::Graphics
{
	class SkinnedMultiMatMesh
	{
	public:
		SkinnedMultiMatMesh(const std::string& filePath);
		~SkinnedMultiMatMesh();

		void Render(Pg::Data::CameraData* camData);
	private:
		void CreateSamplerState();
		void CreateVertexPixelShader();
		void CreateRasterizerState();
		void CreateConstantBuffer();
		void LoadHardCodedSRVs();

		//기존 프레임워크에 의존하지 않는 방식. 
		void ImportSkinnedAsset(const std::string& filePath);

		//해당 FBX에서는 Animation들이 이미 들어 있다. 그걸 활용해 출력하고자 한다.

		//Skinned Mesh Rendering.
		void RenderScene(Pg::Data::CameraData* camData);

		void ReadSceneHierarchy(unsigned int keyframe, const aiScene* scene, aiNode* node, DirectX::XMMATRIX parentTransform, Animation* animation, Skeleton* skeleton);
		const aiNodeAnim* CheckNodeAnim(const aiAnimation* animation, const std::string nodeName);
	private:
		//Assimp / Parsing 직접 처리와 관련된 함수들. (TofuModelViewer)
		Assimp::Importer* _importer;
		const aiScene* scene;

		D3D11_SUBRESOURCE_DATA _cbufferSubresourceData0;
		D3D11_SUBRESOURCE_DATA _cbufferSubresourceData1;
		ID3D11Buffer* _constantBuffer[2]; //인덱스 0은 일반, 인덱스 1은 Bone 기준의 Skinned.
		
		ID3D11SamplerState* _samplerState = nullptr;
		ID3D11RasterizerState* _rasterizerState = nullptr;

		Skeleton* _skeleton = nullptr;

	private:
		Pg::API::Input::PgInput* _tempInput;
	private:
		//Boss_Test_NonDeform_MultiMat.fbx를 렌더하기 위해서, 개별적으로 SRV들 마련. (임베딩X, 작동을 보려고)
		std::array< ID3D11ShaderResourceView*, 3> _tempSRVArray;
		ID3D11ShaderResourceView* _tempTimmySRV;
		ID3D11ShaderResourceView* _tempCylinderSRV;

		ConstantBufferDefine::cbPerObjectBase* _constantBufferStruct;
		ConstantBufferDefine::cbPerObjectSkinned* _skinnedCBuffer;

		ID3D11Device* _device;
		ID3D11DeviceContext* _devCon;

		ID3D11VertexShader* _vertexShader = nullptr;
		ID3D11PixelShader* _pixelShader = nullptr;

	private:
		//Vertex랑 1대1로 대응할, Render
		UINT _numVertices = 0;
		UINT _numIndices = 0;

		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;

	private:
		void GetPositionAtKeyframe(aiVector3D& position, unsigned int keyframe, const aiNodeAnim* nodeAnim);
		void GetRotationAtKeyframe(aiQuaternion& rotation, unsigned int keyframe, const aiNodeAnim* nodeAnim);
		void GetScaleAtKeyframe(aiVector3D& scale, unsigned int keyframe, const aiNodeAnim* nodeAnim);


	};
}


