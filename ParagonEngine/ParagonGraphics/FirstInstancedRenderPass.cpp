#include "FirstInstancedRenderPass.h"
#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "SystemInterfacedVertexShader.h"
#include "SystemPixelShader.h"
#include "Asset3DModelData.h"
#include "AssetModelDataDefine.h"

#include "RenderTexture2DArray.h"

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

		_DXStorage->_deviceContext->ClearDepthStencilView(_d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetDepthStencilState(_d3dCarrierTempStorage->_mainLightGBufDSV->GetDSState(), 0);

		for (auto& e : _d3dCarrierTempStorage->_gBufRequiredRTVArray)
		{
			_DXStorage->_deviceContext->ClearRenderTargetView(e, _DXStorage->_backgroundColor);
		}

		//특수한 상황, Render 안에 bind 존재.
		//VB/IB 바꾸는게 가장 비싸기 때문에. RT / Shader Switching을 주로 쓸 것이다.
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
		_vs = std::make_unique<SystemInterfacedVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_VS_DIRECTORY), LayoutDefine::GetInstanced1stLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 
			std::string("g_ViewProjGetter"), std::initializer_list<std::string>{ std::string("CCameraViewProjGet"), std::string("CMainLightViewProjGet")} );
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_STAGE_PS_DIRECTORY));
		_depthRecordOnlyPS = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_DEPTH_ONLY_STAGE_PS_DIRECTORY));
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

			//Camera 시점 렌더.
			{
				//OMSetRenderTargets - 원래 담았어야 할 곳에 기록!
				_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(),
					_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

				//메인 VS(Camera-Based) / PS 바인드. 
				_vs->Bind(0);
				_ps->Bind();

				// Albedo
				_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(bModel->_pbrTextureArrays[0]->GetSRV()));
				// Normal
				_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(bModel->_pbrTextureArrays[1]->GetSRV()));
				// ARM
				_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(bModel->_pbrTextureArrays[2]->GetSRV()));

				//우선적으로, ConstantBuffer부터 셋한다.
				assert(bBufferPairList->_instancedLightMapSetVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING);

				for (int i = 0; i < bBufferPairList->_instancedLightMapSetVec.size(); i++)
				{
					_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = bBufferPairList->_instancedLightMapSetVec.at(i);
				}

				_lightmapCBuffer->Update();

				//이제 PS CB Bind과정.
				_lightmapCBuffer->BindPS(6);

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

				_lightmapCBuffer->UnbindPS(6);

				ID3D11ShaderResourceView* tNullSRV = nullptr;
				// Albedo
				_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &tNullSRV);
				// Normal
				_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &tNullSRV);
				// ARM
				_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &tNullSRV);
			}
			///이제 VertexBuffer가 아직도 Binding되어 있을 이 상황에서, DynamicLinkage 값만 바꾸자.
			//Light 시점 렌더.
			{
				// Unbind RenderTarget
				_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);

				//Shadow 렌더 위한 스위칭.
				_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
				_vs->Bind(1);
				_ps->Unbind();
				_depthRecordOnlyPS->Bind();

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

				_depthRecordOnlyPS->Unbind();
				_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
				//기록도 다 했으니, 다음을 위한 준비.
			}
			
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

			//Camera 시점 렌더.
			{
				//OMSetRenderTargets - 원래 담았어야 할 곳에 기록!
				_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(),
					_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

				//메인 VS(Camera-Based) / PS 바인드. 
				_vs->Bind(0);
				_ps->Bind();

				// Albedo
				_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(bModel->_pbrTextureArrays[0]->GetSRV()));
				// Normal
				_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(bModel->_pbrTextureArrays[1]->GetSRV()));
				// ARM
				_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(bModel->_pbrTextureArrays[2]->GetSRV()));

				//우선적으로, ConstantBuffer부터 셋한다.
				assert(bBufferPairList->_instancedLightMapSetVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING);

				for (int i = 0; i < bBufferPairList->_instancedLightMapSetVec.size(); i++)
				{
					_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = bBufferPairList->_instancedLightMapSetVec.at(i);
				}

				_lightmapCBuffer->Update();

				//이제 PS CB Bind과정.
				_lightmapCBuffer->BindPS(6);

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

				_lightmapCBuffer->UnbindPS(6);

				ID3D11ShaderResourceView* tNullSRV = nullptr;
				// Albedo
				_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &tNullSRV);
				// Normal
				_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &tNullSRV);
				// ARM
				_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &tNullSRV);
			}
			///이제 VertexBuffer가 아직도 Binding되어 있을 이 상황에서, DynamicLinkage 값만 바꾸자.
			//Light 시점 렌더.
			{
				// Unbind RenderTarget
				_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);

				//Shadow 렌더 위한 스위칭.
				_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
				_vs->Bind(1);
				_ps->Unbind();
				_depthRecordOnlyPS->Bind();

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

				_depthRecordOnlyPS->Unbind();
				_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
				//기록도 다 했으니, 다음을 위한 준비.
			}
		}
	}
}
