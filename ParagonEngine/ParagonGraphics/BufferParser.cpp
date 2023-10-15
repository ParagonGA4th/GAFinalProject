#include "BufferParser.h"
#include "Asset3DModelData.h"
#include "Asset3DModelDefine.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "../ParagonUtil/ByteBuffer.hpp"
#include <vector>
#include <cassert>

namespace Pg::Graphics::Helper
{
	BufferParser::BufferParser()
	{
		//
	}

	BufferParser::~BufferParser()
	{
		//
	}

	void BufferParser::Asset3DModelToD3DBuffer(Pg::Graphics::D3DBufferInfo& outBufferInfo, bool isSkinned, Pg::Graphics::AssetSceneData* assetSceneData)
	{
		if (isSkinned)
		{
			//아직 구현이 되지 않았다. Skinned Model이 이 시점에서 들어오면 안됨.
			assert(false);
			AssetSkinnedModelToD3DBuffer(outBufferInfo, assetSceneData);
		}
		else
		{
			AssetStaticModelToD3DBuffer(outBufferInfo, assetSceneData);
		}
	}

	void BufferParser::AssetStaticModelToD3DBuffer(Pg::Graphics::D3DBufferInfo& outBufferInfo, Pg::Graphics::AssetSceneData* assetSceneData)
	{
		auto&& tD3DBuffer = outBufferInfo;
		Pg::Util::ByteBuffer* tByteVertexBuffer = new Pg::Util::ByteBuffer;

		//전체 VertexCount 세기 + 기존
		size_t tVertexCount = 0;
		unsigned int tDrawStartVertexCnt = 0;

		//Mesh 개수 전달.
		tD3DBuffer._meshCount = assetSceneData->m_NumMesh;

		//Material ID 전달. (Mesh와 1대1 대응하게)
		for (size_t i = 0; i < assetSceneData->m_NumMesh; i++)
		{
			tD3DBuffer._materialIDVector.push_back(assetSceneData->m_MeshList[i]->m_MaterialIndex);
		}

		for (size_t i = 0; i < assetSceneData->m_NumMesh; i++)
		{
			tD3DBuffer._vertexOffsetVector.push_back(tDrawStartVertexCnt);

			//Mesh별 시작 Vertex Cnt 기록 w/ 총 Vertex 개수 기록.
			tVertexCount += assetSceneData->m_MeshList[i]->m_NumVertice;

			tDrawStartVertexCnt = tVertexCount;
		}

		//InputLayout에 따라서 3DModel의 정보 로드. (Vin1stStatic)
		for (size_t i = 0; i < assetSceneData->m_NumMesh; i++)
		{
			for (size_t j = 0; j < assetSceneData->m_MeshList[i]->m_NumVertice; j++)
			{
				//posL
				tByteVertexBuffer->putBytes((uint8_t*)&(assetSceneData->m_MeshList[i]->m_VerticeList[j]), static_cast<uint32_t>(sizeof(DirectX::XMFLOAT3)));
				//normalL
				tByteVertexBuffer->putBytes((uint8_t*)&(assetSceneData->m_MeshList[i]->m_NormalList[j]), static_cast<uint32_t>(sizeof(DirectX::XMFLOAT3)));
				//tangentL
				tByteVertexBuffer->putBytes((uint8_t*)&(assetSceneData->m_MeshList[i]->m_TangentList[j]), static_cast<uint32_t>(sizeof(DirectX::XMFLOAT3)));
				//color
				tByteVertexBuffer->putBytes((uint8_t*)&(assetSceneData->m_MeshList[i]->m_ColorList[j]), static_cast<uint32_t>(sizeof(DirectX::XMFLOAT4)));
				//tex
				tByteVertexBuffer->putBytes((uint8_t*)&(assetSceneData->m_MeshList[i]->m_TextureCoordList[j]), static_cast<uint32_t>(sizeof(DirectX::XMFLOAT3)));
				//matID
				tByteVertexBuffer->putBytes((uint8_t*)&(assetSceneData->m_MeshList[i]->m_MaterialIndex), static_cast<uint32_t>(sizeof(unsigned int)));
			}
		}

		//전체 IndexCount 세기 + 기존
		size_t tIndexCount = 0;
		unsigned int tDrawStartIndexCnt = 0;

		for (size_t i = 0; i < assetSceneData->m_NumMesh; i++)
		{
			tD3DBuffer._indexOffsetVector.push_back(tDrawStartIndexCnt);

			//Mesh별 시작 Index Cnt 기록 w/ 총 Index 개수 기록.
			for (size_t j = 0; j < assetSceneData->m_MeshList[i]->m_NumFace; j++)
			{
				tIndexCount += assetSceneData->m_MeshList[i]->m_FaceList[j].m_NumIndice;
			}

			tDrawStartIndexCnt = tIndexCount;
		}

		std::vector<unsigned int> tIBVec;
		tIBVec.reserve(tIndexCount);

		for (auto& it : assetSceneData->m_MeshList)
		{
			for (auto&& itt : it->m_FaceList)
			{
				for (int i = 0; i < itt.m_IndiceList.size(); i++)
				{
					tIBVec.push_back(itt.m_IndiceList[i]);
				}
			}
		}

		D3D11_BUFFER_DESC tVBD;
		tVBD.Usage = D3D11_USAGE_IMMUTABLE;
		tVBD.ByteWidth = static_cast<UINT>(sizeof(LayoutDefine::Vin1stStatic) * tVertexCount);
		tVBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		tVBD.CPUAccessFlags = 0;
		tVBD.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = tByteVertexBuffer->GetStartAddress();

		HRESULT hr = LowDX11Storage::GetInstance()->_device->CreateBuffer(&tVBD, &vinitData, &(tD3DBuffer._vertexBuffer));
		if (FAILED(hr))
		{
			assert(false);
		}

		D3D11_BUFFER_DESC tIBD;
		tIBD.Usage = D3D11_USAGE_IMMUTABLE;
		tIBD.ByteWidth = static_cast<UINT>(sizeof(UINT) * tIndexCount);
		tIBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
		tIBD.CPUAccessFlags = 0;
		tIBD.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = tIBVec.data();

		hr = LowDX11Storage::GetInstance()->_device->CreateBuffer(&tIBD, &iinitData, &(tD3DBuffer._indexBuffer));

		tD3DBuffer._indexCount = static_cast<UINT>(tIndexCount);

		//메모리 릭 막기
		delete tByteVertexBuffer;
	}

	void BufferParser::AssetSkinnedModelToD3DBuffer(Pg::Graphics::D3DBufferInfo& outBufferInfo, Pg::Graphics::AssetSceneData* assetSceneData)
	{
		//아직 구현 안됨.
	}

}