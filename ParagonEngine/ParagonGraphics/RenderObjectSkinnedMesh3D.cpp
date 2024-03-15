#include "RenderObjectSkinnedMesh3D.h"

#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "LowDX11Storage.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "LayoutDefine.h"
#include "MathHelper.h"
#include "RenderTexture2D.h"
#include "AssetModelDataDefine.h"
#include "AssetTextureType.h"
#include "MaterialCluster.h"
#include "../ParagonData/ParagonDefines.h"
#include "dxtk/WICTextureLoader.h"
#include <cassert>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;


	RenderObjectSkinnedMesh3D::RenderObjectSkinnedMesh3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID, unsigned int matID) : RenderObject3D(baseRenderer, objID, matID)
	{
		//SkinnedMesh도 똑같은 과정 거쳐야.
		//StaticMeshRenderer 따로 포인터를 받기.
		//Mesh 데이터를 받기.
		//VB/IB 로드.
		//Constant Buffer Data를 생성.

		//Bone의 수만큼 GPU에 들어갈 벡터의 크기를 설정해야 한다. (ASSET_MAXIMUM_BONE_NUMBER_PER_MESH)
		_boneTransformVector.resize(Pg::Defines::ASSET_MAXIMUM_BONE_NUMBER_PER_MESH);
		std::fill(_boneTransformVector.begin(), _boneTransformVector.end(), DirectX::SimpleMath::Matrix::Identity);

	}

	RenderObjectSkinnedMesh3D::~RenderObjectSkinnedMesh3D()
	{
		//delete _constantBufferStruct;
	}

	void RenderObjectSkinnedMesh3D::CreateObjMatBuffers()
	{

	}

	void RenderObjectSkinnedMesh3D::First_Render(const float* const dt)
	{
		////버퍼 바인딩.
		//BindBuffers();
		//_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//
		//int tMeshCount = _modelData->_assetSceneData->_totalMeshCount;
		//
		//for (int i = 0; i < tMeshCount; i++)
		//{
		//	//MultiMesh -> Material 적용할 수 있게 여기서도 Vector Clear.
		//	_textures.clear();
		//
		//	UINT tToDrawIndexCount = _modelData->_assetSceneData->_meshList[i]._numIndices;
		//	UINT tMatID = _modelData->_assetSceneData->_meshList[i]._materialID;
		//
		//	//이거 한번만 받아도 되겠지만, 일단은 통일성을 위해서.
		//	//아니면 업데이트되는 로직을 여기랑 연관? 후의 일.
		//	this->_diffuse = _modelData->GetMaterialByIndex(tMatID)->GetTextureByType(PG_TextureType_DIFFUSE);
		//	this->_normal = _modelData->GetMaterialByIndex(tMatID)->GetTextureByType(PG_TextureType_NORMALS);
		//
		//	if (this->_diffuse == nullptr)
		//	{
		//		this->_diffuse = GraphicsResourceManager::Instance()->GetDefaultTexture(PG_TextureType_DIFFUSE);
		//	}
		//	if (this->_normal == nullptr)
		//	{
		//		this->_normal = GraphicsResourceManager::Instance()->GetDefaultTexture(PG_TextureType_NORMALS);
		//	}
		//
		//	_textures.emplace_back(_diffuse);
		//	_textures.emplace_back(_normal);
		//
		//	BindTextures();
		//
		//	//업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
		//	_devCon->DrawIndexed(tToDrawIndexCount,
		//		_modelData->_assetSceneData->_meshList[i]._indexOffset,
		//		_modelData->_assetSceneData->_meshList[i]._vertexOffset);
		//}
	}

	void RenderObjectSkinnedMesh3D::First_BindBuffers()
	{
		////Vertex Buffer Setting.
		//UINT stride = sizeof(LayoutDefine::Vin1stSkinned);
		//UINT offset = 0;
		//_devCon->IASetVertexBuffers(0, 1, &(_modelData->_vertexBuffer), &stride, &offset);
		//
		////Index Buffer Setting.
		//_devCon->IASetIndexBuffer(_modelData->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void RenderObjectSkinnedMesh3D::First_UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{

	}

	void RenderObjectSkinnedMesh3D::First_UnbindBuffers()
	{

	}

	void RenderObjectSkinnedMesh3D::ObjMat_UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{

	}

	void RenderObjectSkinnedMesh3D::ObjMat_BindBuffers()
	{

	}

	void RenderObjectSkinnedMesh3D::ObjMat_Render(const float* const dt)
	{

	}

	void RenderObjectSkinnedMesh3D::ObjMat_UnbindBuffers()
	{

	}

	void RenderObjectSkinnedMesh3D::SetAnimation(const std::string& animName)
	{
		assert(_modelData->_assetSkinnedData->_viableAnimations.contains(animName) && 
			"걸리면 유효하지 않은 Animation 로드 시도한 것");


	}

}