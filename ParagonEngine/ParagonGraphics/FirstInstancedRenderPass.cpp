#include "FirstInstancedRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "Asset3DModelData.h"
#include "AssetModelDataDefine.h"

namespace Pg::Graphics
{
	FirstInstancedRenderPass::FirstInstancedRenderPass() : _DXStorage(LowDX11Storage::GetInstance())
	{
		CreateShaders();
	}

	FirstInstancedRenderPass::~FirstInstancedRenderPass()
	{

	}

	void FirstInstancedRenderPass::Initialize()
	{
		//이는 Scene이 바뀔 때마다 다시 호출될 것이다.	

		//이제 나머지 Initialize.
		_lightmapCBuffer.reset(new ConstantBuffer<ConstantBufferDefine::cbLightmapCollection>());
		for (int i = 0; i < Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING; i++)
		{
			_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = SingleLightMapSet({ 1.f,1.f }, { 0.f,0.f }, 0);
		}
	}

	void FirstInstancedRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_d3dCarrierTempStorage = &carrier;
	}

	void FirstInstancedRenderPass::BindPass()
	{
		//자체적인 DSV Clear, Depth Stencil State 리셋, OMSetRenderTargets.
		_DXStorage->_deviceContext->ClearDepthStencilView(_d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetDepthStencilState(_d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSState(), 0);

		for (auto& e : _d3dCarrierTempStorage->_gBufRequiredRTVArray)
		{
			_DXStorage->_deviceContext->ClearRenderTargetView(e, _DXStorage->_backgroundColor);
		}

		//자신을 위한 Bind.
		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(),
			_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

		// 셰이더 바인딩.
		_vs->Bind();
		_ps->Bind();

	}

	void FirstInstancedRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderNormalInstanced(renderObjectList, camData);
		RenderCulledOppositeInstanced(renderObjectList, camData);
	}

	void FirstInstancedRenderPass::UnbindPass()
	{
		// Unbind RenderTarget
		_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);

		// Unbind Shaders
		_vs->Unbind();
		_ps->Unbind();
	}

	void FirstInstancedRenderPass::ExecuteNextRenderRequirements()
	{
		//
	}

	void FirstInstancedRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{
		//
	}

	void FirstInstancedRenderPass::CreateShaders()
	{
		using Pg::Util::Helper::ResourceHelper;

		// Instanced Pass
		_vs = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_VS_DIRECTORY), LayoutDefine::GetInstanced1stLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_STAGE_PS_DIRECTORY));
	}

	void FirstInstancedRenderPass::RenderNormalInstanced(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);
		

		for (auto& [bModel, bBufferPairList] : tRenderObjectList->_instancedStaticList)
		{
			//만약 렌더할게 비어있다면, continue.
			if (bBufferPairList->_instancedStaticPairVec.empty())
			{
				continue;
			}

			//우선적으로, ConstantBuffer부터 셋한다.
			assert(bBufferPairList->_instancedLightMapSetVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING);

			for (int i = 0; i < bBufferPairList->_instancedLightMapSetVec.size(); i++)
			{
				_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = bBufferPairList->_instancedLightMapSetVec.at(i);
			}
			
			_lightmapCBuffer->Update();

			//이제 PS CB Bind과정.
			_lightmapCBuffer->BindPS(5);

			//Vertex / Index Buffer Setting.
			//1st :  Vin1stStatic_Individual
			//2nd :	 Vin2ndAll_Individual
			//3rd :  Vin3rdStaticSkinned_Individual

			UINT strides[3] = { sizeof(LayoutDefine::Vin1stStatic_Individual), sizeof(LayoutDefine::Vin2ndAll_Individual), sizeof(LayoutDefine::Vin3rdInstanced_Individual) };
			UINT offsets[3] = { 0,0,0 };
			ID3D11Buffer* vbArray[3] = { bModel->_vertexBuffer , bModel->_secondVertexBuffer, bBufferPairList->_vb }; // 이렇게 세 개의 오브젝트 버퍼를 모두 정렬 및 처리 완료.

			//
			_DXStorage->_deviceContext->IASetVertexBuffers(0, 3, vbArray, strides, offsets);

			//Index Buffer Setting.
			_DXStorage->_deviceContext->IASetIndexBuffer(bModel->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			//그리기.
			//DrawIndexedInstanced를 사용.
			int tMeshCount = bModel->_assetSceneData->_totalMeshCount;
			for (int i = 0; i < tMeshCount; i++)
			{
				//MultiMesh -> Material 적용할 수 있게 여기서도 Vector Clear.
				UINT tToDrawIndexCount = bModel->_assetSceneData->_meshList[i]._numIndices;

				//업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
				_DXStorage->_deviceContext->DrawIndexedInstanced(tToDrawIndexCount, bBufferPairList->_instancedStaticPairVec.size(),
					bModel->_assetSceneData->_meshList[i]._indexOffset,
					bModel->_assetSceneData->_meshList[i]._vertexOffset, 0);
			}

			_lightmapCBuffer->UnbindPS(5);
		}
	}

	void FirstInstancedRenderPass::RenderCulledOppositeInstanced(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);

		//렌더하기 전에, Rasterizer State Cull 반대로 돌리면 된다. 얘는 반대로 컬링홰야 하는 친구이니.
		_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidFrontfaceCullingState);

		for (auto& [bModel, bBufferPairList] : tRenderObjectList->_instancedCulledOppositeStaticList)
		{
			//만약 렌더할게 비어있다면, continue.
			if (bBufferPairList->_instancedStaticPairVec.empty())
			{
				continue;
			}

			//우선적으로, ConstantBuffer부터 셋한다.
			assert(bBufferPairList->_instancedLightMapSetVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING);

			for (int i = 0; i < bBufferPairList->_instancedLightMapSetVec.size(); i++)
			{
				_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = bBufferPairList->_instancedLightMapSetVec.at(i);
			}

			_lightmapCBuffer->Update();

			//이제 PS CB Bind과정.
			_lightmapCBuffer->BindPS(5);

			//Vertex / Index Buffer Setting.
			//1st :  Vin1stStatic_Individual
			//2nd :	 Vin2ndAll_Individual
			//3rd :  Vin3rdStaticSkinned_Individual

			UINT strides[3] = { sizeof(LayoutDefine::Vin1stStatic_Individual), sizeof(LayoutDefine::Vin2ndAll_Individual), sizeof(LayoutDefine::Vin3rdInstanced_Individual) };
			UINT offsets[3] = { 0,0,0 };
			ID3D11Buffer* vbArray[3] = { bModel->_vertexBuffer , bModel->_secondVertexBuffer, bBufferPairList->_vb }; // 이렇게 세 개의 오브젝트 버퍼를 모두 정렬 및 처리 완료.

			//
			_DXStorage->_deviceContext->IASetVertexBuffers(0, 3, vbArray, strides, offsets);

			//Index Buffer Setting.
			_DXStorage->_deviceContext->IASetIndexBuffer(bModel->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			//그리기.
			//DrawIndexedInstanced를 사용.
			int tMeshCount = bModel->_assetSceneData->_totalMeshCount;
			for (int i = 0; i < tMeshCount; i++)
			{
				//MultiMesh -> Material 적용할 수 있게 여기서도 Vector Clear.
				UINT tToDrawIndexCount = bModel->_assetSceneData->_meshList[i]._numIndices;

				//업데이트된 다음에 호출된 해당 Mesh만큼 그린다.
				_DXStorage->_deviceContext->DrawIndexedInstanced(tToDrawIndexCount, bBufferPairList->_instancedStaticPairVec.size(),
					bModel->_assetSceneData->_meshList[i]._indexOffset,
					bModel->_assetSceneData->_meshList[i]._vertexOffset, 0);
			}

			_lightmapCBuffer->UnbindPS(5);
		}
	}

}
