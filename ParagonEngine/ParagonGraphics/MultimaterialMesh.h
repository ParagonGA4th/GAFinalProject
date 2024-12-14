//#pragma once
//#include "ConstantBufferDefine.h"
//#include <string>
//#include <map>
//#include <vector>
//#include <unordered_map>
//#include <array>
//#include <DirectXMath.h>
//#include <d3d11.h>
//#include <dxtk/SimpleMath.h>
//
//#include "RenderPrepStructs.h"
//
///// <summary>
///// Deferred Rendering이 전역적으로 자리잡기 전에, 
///// 다중 Material 적용되는 Mesh 렌더 데모를 위해. -> 이제 Skinned 용도로 개조!
///// Boss_Test_NonDeform_MultiMat.fbx 연동하기 위해!
///// </summary>
//
//struct aiNode;
//struct aiMesh;
//struct aiScene;
//struct aiAnimation;
//struct aiNodeAnim;
//
//namespace Assimp
//{
//	class Importer;
//}
//
//namespace Pg::Util
//{
//	namespace Input
//	{
//		class InputSystem;
//	}
//}
//namespace Pg::Data
//{
//	struct CameraData;
//}
//
//namespace Pg::Graphics
//{
//	class Asset3DModelData;
//}
//
//namespace Pg::Graphics
//{
//	class MultimaterialMesh
//	{
//
//	public:
//		MultimaterialMesh(const std::string& filePath);
//		~MultimaterialMesh();
//
//		void Render(Pg::Data::CameraData* camData);
//	private:
//		void CreateSamplerState();
//		void CreateVertexPixelShader();
//		void CreateRasterizerState();
//		void CreateConstantBuffer();
//		void LoadHardCodedSRVs();
//
//		//기존 프레임워크에 의존하지 않는 방식. 
//		void ImportSkinnedAsset(const std::string& filePath);
//				
//		//해당 FBX에서는 Animation들이 이미 들어 있다. 그걸 활용해 출력하고자 한다.
//
//		//Skinned Mesh Rendering.
//		void RenderScene(Pg::Data::CameraData* camData);
//
//	private:
//		void SetupRenderUsageMesh(); //Vertex의 시작점 / Index Count / Index의 시작점 기록.
//
//	private:
//		//Assimp / Parsing 직접 처리와 관련된 함수들. (TofuModelViewer)
//		Assimp::Importer* _importer;
//		const aiScene* scene;
//
//		DirectX::SimpleMath::Matrix _meshGlobalInverseTransform;
//
//		ID3D11Buffer* _vertexBuffer;
//		ID3D11Buffer* _indexBuffer;
//		uint32_t			_numVertices;
//		uint32_t			_numIndices;
//
//		std::vector<Mesh>	meshes;
//
//	private:
//		void RenderSkinnedNodes(Pg::Data::CameraData* camData);
//	private:
//		//Bone 셋업 작업.
//		void SetupBoneData(std::vector<RenderPrepVertexBone>& vBoneList, const aiScene* scene);
//		//Bone Render용 Bone 셋업 작업.
//		void SetupRenderBones(unsigned int index, aiMesh* mesh, std::vector<RenderPrepVertexBone>& vBoneList);
//	
//		std::map<std::string, unsigned int> _mappedBones;
//		unsigned int _formationNumBone = 0;
//
//		std::vector<RenderPrepVertexBone> _vertexBoneVector;
//		std::vector<RenderPrepMesh> _meshEntriesVector;
//		std::vector<RenderPrepBoneInfo> _renderBoneInfoVector;
//
//		//Animation UpdateBuffer.
//		std::vector<DirectX::SimpleMath::Matrix> _boneTransformVector;
//
//	private:
//		void BoneTransformUpdate();
//
//		void ReadNodeHierarchy(double animTick, const aiNode* pNode, const aiAnimation* pAnim, DirectX::SimpleMath::Matrix parentTransform);
//		void CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& outQuat, double animTick, const aiNodeAnim* pNodeAnim);
//		void CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& outVec, double animTick, const aiNodeAnim* pNodeAnim);
//
//		unsigned int FindRotation(double animTick, const aiNodeAnim* pNodeAnim);
//		unsigned int FindTranslation(double animTick, const aiNodeAnim* pNodeAnim);
//
//	private:
//		Pg::Util::Input::InputSystem* _tempInput;
//	private:
//		//Boss_Test_NonDeform_MultiMat.fbx를 렌더하기 위해서, 개별적으로 SRV들 마련. (임베딩X, 작동을 보려고)
//		std::array< ID3D11ShaderResourceView*, 3> _tempSRVArray;
//		ID3D11ShaderResourceView* _tempTimmySRV;
//		ID3D11ShaderResourceView* _tempCylinderSRV;
//		ID3D11ShaderResourceView* _temp4QSRV;
//		ID3D11ShaderResourceView* _tempFallFlatMonsterSRV;
//
//
//	private:
//		void UpdateConstantBufferBase(Pg::Data::CameraData* camData, DirectX::XMFLOAT4X4 worldMat);
//	private:
//		Asset3DModelData* _modelData = nullptr;
//		ConstantBufferDefine::cbPerObjectBase* _constantBufferStruct;
//		ConstantBufferDefine::cbPerObjectSkinnedBones* _skinnedCBuffer;
//
//	private:
//		ID3D11Device* _device;
//		ID3D11DeviceContext* _devCon;
//
//		ID3D11VertexShader* _vertexShader = nullptr;
//		ID3D11PixelShader* _pixelShader = nullptr;
//
//		D3D11_SUBRESOURCE_DATA _cbufferSubresourceData0;
//		D3D11_SUBRESOURCE_DATA _cbufferSubresourceData1;
//		ID3D11Buffer* _constantBuffer[2]; //인덱스 0은 일반, 인덱스 1은 Bone 기준의 Skinned.
//
//		ID3D11SamplerState* _samplerState = nullptr;
//		ID3D11RasterizerState* _rasterizerState = nullptr;
//	private:
//		//SRV 자체의 문제인지 테스트 위해서, SRV를 자체적으로 만듬.
//		ID3D11ShaderResourceView* _testSRV = nullptr;
//		ID3D11Resource* _testResource = nullptr;
//	};
//}
//
//
