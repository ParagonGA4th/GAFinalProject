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
		//�̴� Scene�� �ٲ� ������ �ٽ� ȣ��� ���̴�.	

		//���� ������ Initialize.
		_lightmapCBuffer.reset(new ConstantBuffer<ConstantBufferDefine::cbLightmapCollection>());
		for (int i = 0; i < Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING; i++)
		{
			_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = SingleLightMapSet({ 1.f,1.f }, { 0.f,0.f }, 0);
		}

		_switchableViewProjCBuffer.reset(new ConstantBuffer<ConstantBufferDefine::cbSwitchableViewProj>());
	}

	void FirstInstancedRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		_d3dCarrierTempStorage = &carrier;
	}

	void FirstInstancedRenderPass::BindPass()
	{
		//��ü���� DSV Clear, Depth Stencil State ����, OMSetRenderTargets.
		_DXStorage->_deviceContext->ClearDepthStencilView(_d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetDepthStencilState(_d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSState(), 0);

		const float tColor[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
		_DXStorage->_deviceContext->ClearRenderTargetView(_d3dCarrierTempStorage->_mainLightGBufRT->GetRTV(), tColor);
		_DXStorage->_deviceContext->ClearDepthStencilView(_d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		_DXStorage->_deviceContext->OMSetDepthStencilState(_d3dCarrierTempStorage->_mainLightGBufDSV->GetDSState(), 0);

		for (auto& e : _d3dCarrierTempStorage->_gBufRequiredRTVArray)
		{
			_DXStorage->_deviceContext->ClearRenderTargetView(e, _DXStorage->_backgroundColor);
		}

		//Ư���� ��Ȳ, Render �ȿ� bind ����.
		//VB/IB �ٲٴ°� ���� ��α� ������. RT / Shader Switching�� �ַ� �� ���̴�.
		_vs->Bind();
	}

	void FirstInstancedRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{

		RenderNormalInstanced(renderObjectList, camData);
		RenderCulledOppositeInstanced(renderObjectList, camData);

		RenderAlphaClippedInstanced(renderObjectList, camData);
		RenderAlphaClippedCulledOppositeInstanced(renderObjectList, camData);

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
		//_vs = std::make_unique<SystemInterfacedVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_VS_DIRECTORY), LayoutDefine::GetInstanced1stLayout(),
		//	LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 
		//	std::string("g_ViewProjGetter"), std::initializer_list<std::string>{ std::string("CCameraViewProjGet"), std::string("CMainLightViewProjGet")} );
		_vs = std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_VS_DIRECTORY), LayoutDefine::GetInstanced1stLayout(),
			LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ps = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_STAGE_PS_DIRECTORY));
		_depthRecordOnlyPS = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_INSTANCED_DEPTH_ONLY_STAGE_PS_DIRECTORY));
		_alphaClippedPS = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_ALPHA_CLIPPING_PS_DIRECTORY));
		_alphaClippedDepthRecordOnlyPS = std::make_unique<SystemPixelShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::FIRST_ALPHA_CLIPPING_INSTANCED_DEPTH_ONLY_STAGE_PS_DIRECTORY));
	}

	void FirstInstancedRenderPass::RenderNormalInstanced(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);


		for (auto& [bModel, bBufferPairList] : tRenderObjectList->_instancedStaticList)
		{
			//���� �����Ұ� ����ִٸ�, continue.
			if (bBufferPairList->_instancedStaticPairVec.empty())
			{
				continue;
			}

			_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_defaultViewport));

			_switchableViewProjCBuffer->GetDataStruct()->_viewProj = Pg::Math::PG2XM_MATRIX4X4(camData->_viewMatrix * camData->_projMatrix);
			_switchableViewProjCBuffer->Update();
			_switchableViewProjCBuffer->BindVS(1);

			//Camera ���� ����.
			{
				//OMSetRenderTargets - ���� ��Ҿ�� �� ���� ���!
				_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(),
					_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

				//���� VS(Camera-Based) / PS ���ε�. 
				_ps->Bind();

				//�׳� �������. Lighting Check.
				// Albedo
				_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(bModel->_pbrTextureArrays[0]->GetSRV()));
				//// Normal
				_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(bModel->_pbrTextureArrays[1]->GetSRV()));
				// ARM
				_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(bModel->_pbrTextureArrays[2]->GetSRV()));

				//�켱������, ConstantBuffer���� ���Ѵ�.
				assert(bBufferPairList->_instancedLightMapSetVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING);

				for (int i = 0; i < bBufferPairList->_instancedLightMapSetVec.size(); i++)
				{
					_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = bBufferPairList->_instancedLightMapSetVec.at(i);
				}

				_lightmapCBuffer->Update();

				//���� PS CB Bind����.
				_lightmapCBuffer->BindPS(6);

				//Vertex / Index Buffer Setting.
				//1st :  Vin1stStatic_Individual
				//2nd :	 Vin2ndAll_Individual
				//3rd :  Vin3rdStaticSkinned_Individual

				UINT strides[3] = { sizeof(LayoutDefine::Vin1stStatic_Individual), sizeof(LayoutDefine::Vin2ndAll_Individual), sizeof(LayoutDefine::Vin3rdInstanced_Individual) };
				UINT offsets[3] = { 0,0,0 };
				ID3D11Buffer* vbArray[3] = { bModel->_vertexBuffer , bModel->_secondVertexBuffer, bBufferPairList->_vb }; // �̷��� �� ���� ������Ʈ ���۸� ��� ���� �� ó�� �Ϸ�.

				//
				_DXStorage->_deviceContext->IASetVertexBuffers(0, 3, vbArray, strides, offsets);

				//Index Buffer Setting.
				_DXStorage->_deviceContext->IASetIndexBuffer(bModel->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

				//�׸���.
				//DrawIndexedInstanced�� ���.
				int tMeshCount = bModel->_assetSceneData->_totalMeshCount;
				for (int i = 0; i < tMeshCount; i++)
				{
					//MultiMesh -> Material ������ �� �ְ� ���⼭�� Vector Clear.
					UINT tToDrawIndexCount = bModel->_assetSceneData->_meshList[i]._numIndices;

					//������Ʈ�� ������ ȣ��� �ش� Mesh��ŭ �׸���.
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

			/////���� VertexBuffer�� ������ Binding�Ǿ� ���� �� ��Ȳ����, ViewProj Switching. ���� �ٲ���.
			_switchableViewProjCBuffer->UnbindVS(1);
			_ps->Unbind();
			//_switchableViewProjCBuffer->GetDataStruct()->_viewProj = _d3dCarrierTempStorage->_mainLightPerspectiveViewProjMatrix;
			//_switchableViewProjCBuffer->Update();
			//_switchableViewProjCBuffer->BindVS(1);
			//_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_shadowMapViewport));


			//Light ���� ����.
			{
				// Unbind RenderTarget
				//_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);
				//
				////Shadow ���� ���� ����Ī.
				////_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
				//ID3D11RenderTargetView* tEmptyRenderTargets[1] = { _d3dCarrierTempStorage->_mainLightGBufRT->GetRTV() };
				//_DXStorage->_deviceContext->OMSetRenderTargets(1, tEmptyRenderTargets, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
				//
				//_ps->Unbind();
				//_depthRecordOnlyPS->Bind();
				//
				//int tMeshCount = bModel->_assetSceneData->_totalMeshCount;
				//for (int i = 0; i < tMeshCount; i++)
				//{
				//	//MultiMesh -> Material ������ �� �ְ� ���⼭�� Vector Clear.
				//	UINT tToDrawIndexCount = bModel->_assetSceneData->_meshList[i]._numIndices;
				//
				//	//������Ʈ�� ������ ȣ��� �ش� Mesh��ŭ �׸���.
				//	_DXStorage->_deviceContext->DrawIndexedInstanced(tToDrawIndexCount, bBufferPairList->_instancedStaticPairVec.size(),
				//		bModel->_assetSceneData->_meshList[i]._indexOffset,
				//		bModel->_assetSceneData->_meshList[i]._vertexOffset, 0);
				//}
				//
				//_switchableViewProjCBuffer->UnbindVS(1);
				//_depthRecordOnlyPS->Unbind();
				_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
				//��ϵ� �� ������, ������ ���� �غ�.
			}

		}
	}

	void FirstInstancedRenderPass::RenderCulledOppositeInstanced(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);

		//�����ϱ� ����, Rasterizer State Cull �ݴ�� ������ �ȴ�. ��� �ݴ�� �ø�ȳ�� �ϴ� ģ���̴�.
		_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidFrontfaceCullingState);

		for (auto& [bModel, bBufferPairList] : tRenderObjectList->_instancedCulledOppositeStaticList)
		{
			//���� �����Ұ� ����ִٸ�, continue.
			if (bBufferPairList->_instancedStaticPairVec.empty())
			{
				continue;
			}

			_switchableViewProjCBuffer->GetDataStruct()->_viewProj = Pg::Math::PG2XM_MATRIX4X4(camData->_viewMatrix * camData->_projMatrix);
			_switchableViewProjCBuffer->Update();
			_switchableViewProjCBuffer->BindVS(1);
			_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_defaultViewport));

			//Camera ���� ����.
			{
				//OMSetRenderTargets - ���� ��Ҿ�� �� ���� ���!
				_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(),
					_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

				//���� VS(Camera-Based) / PS ���ε�. 
				_ps->Bind();

				//������ �׽�Ʈ : �׳� ����α�.
				// Albedo
				_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(bModel->_pbrTextureArrays[0]->GetSRV()));
				// Normal
				_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(bModel->_pbrTextureArrays[1]->GetSRV()));
				// ARM
				_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(bModel->_pbrTextureArrays[2]->GetSRV()));

				//�켱������, ConstantBuffer���� ���Ѵ�.
				assert(bBufferPairList->_instancedLightMapSetVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING);

				for (int i = 0; i < bBufferPairList->_instancedLightMapSetVec.size(); i++)
				{
					_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = bBufferPairList->_instancedLightMapSetVec.at(i);
				}

				_lightmapCBuffer->Update();

				//���� PS CB Bind����.
				_lightmapCBuffer->BindPS(6);

				//Vertex / Index Buffer Setting.
				//1st :  Vin1stStatic_Individual
				//2nd :	 Vin2ndAll_Individual
				//3rd :  Vin3rdStaticSkinned_Individual

				UINT strides[3] = { sizeof(LayoutDefine::Vin1stStatic_Individual), sizeof(LayoutDefine::Vin2ndAll_Individual), sizeof(LayoutDefine::Vin3rdInstanced_Individual) };
				UINT offsets[3] = { 0,0,0 };
				ID3D11Buffer* vbArray[3] = { bModel->_vertexBuffer , bModel->_secondVertexBuffer, bBufferPairList->_vb }; // �̷��� �� ���� ������Ʈ ���۸� ��� ���� �� ó�� �Ϸ�.

				//
				_DXStorage->_deviceContext->IASetVertexBuffers(0, 3, vbArray, strides, offsets);

				//Index Buffer Setting.
				_DXStorage->_deviceContext->IASetIndexBuffer(bModel->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

				//�׸���.
				//DrawIndexedInstanced�� ���.
				int tMeshCount = bModel->_assetSceneData->_totalMeshCount;
				for (int i = 0; i < tMeshCount; i++)
				{
					//MultiMesh -> Material ������ �� �ְ� ���⼭�� Vector Clear.
					UINT tToDrawIndexCount = bModel->_assetSceneData->_meshList[i]._numIndices;

					//������Ʈ�� ������ ȣ��� �ش� Mesh��ŭ �׸���.
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

			///���� VertexBuffer�� ������ Binding�Ǿ� ���� �� ��Ȳ����, ViewProj Switching. ���� �ٲ���.
			_switchableViewProjCBuffer->UnbindVS(1);
			//_switchableViewProjCBuffer->GetDataStruct()->_viewProj = _d3dCarrierTempStorage->_mainLightPerspectiveViewProjMatrix;
			//_switchableViewProjCBuffer->Update();
			//_switchableViewProjCBuffer->BindVS(1);
			//_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_shadowMapViewport));

			//Light ���� ����.
			{
				// Unbind RenderTarget
				//_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);
				//
				////Shadow ���� ���� ����Ī.
				////_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
				////_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_d3dCarrierTempStorage->_mainLightGBufRT->GetRTV()), _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
				//ID3D11RenderTargetView* tEmptyRenderTargets[1] = { _d3dCarrierTempStorage->_mainLightGBufRT->GetRTV() };
				//_DXStorage->_deviceContext->OMSetRenderTargets(1, tEmptyRenderTargets, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());

				//_vs->Bind(1);
				_ps->Unbind();
				//_depthRecordOnlyPS->Bind();
				//
				//int tMeshCount = bModel->_assetSceneData->_totalMeshCount;
				//for (int i = 0; i < tMeshCount; i++)
				//{
				//	//MultiMesh -> Material ������ �� �ְ� ���⼭�� Vector Clear.
				//	UINT tToDrawIndexCount = bModel->_assetSceneData->_meshList[i]._numIndices;
				//
				//	//������Ʈ�� ������ ȣ��� �ش� Mesh��ŭ �׸���.
				//	_DXStorage->_deviceContext->DrawIndexedInstanced(tToDrawIndexCount, bBufferPairList->_instancedStaticPairVec.size(),
				//		bModel->_assetSceneData->_meshList[i]._indexOffset,
				//		bModel->_assetSceneData->_meshList[i]._vertexOffset, 0);
				//}
				//
				//_switchableViewProjCBuffer->UnbindVS(1);
				//_depthRecordOnlyPS->Unbind();
				_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
				//��ϵ� �� ������, ������ ���� �غ�.
			}
		}
	}

	void FirstInstancedRenderPass::RenderAlphaClippedInstanced(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);

		_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidNoCullingState);

		for (auto& [bModel, bBufferPairList] : tRenderObjectList->_instancedStaticAlphaClippedList)
		{
			//���� �����Ұ� ����ִٸ�, continue.
			if (bBufferPairList->_instancedStaticPairVec.empty())
			{
				continue;
			}

			_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_defaultViewport));
			_switchableViewProjCBuffer->GetDataStruct()->_viewProj = Pg::Math::PG2XM_MATRIX4X4(camData->_viewMatrix * camData->_projMatrix);
			_switchableViewProjCBuffer->Update();
			_switchableViewProjCBuffer->BindVS(1);

			//Camera ���� ����.
			{
				//OMSetRenderTargets - ���� ��Ҿ�� �� ���� ���!
				_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(),
					_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

				//���� VS(Camera-Based) / PS ���ε�. 
				_alphaClippedPS->Bind();

				// Albedo
				_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(bModel->_pbrTextureArrays[0]->GetSRV()));
				//// Normal 
				//_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(bModel->_pbrTextureArrays[1]->GetSRV()));
				//// ARM
				//_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(bModel->_pbrTextureArrays[2]->GetSRV()));
				// Alpha
				_DXStorage->_deviceContext->PSSetShaderResources(11, 1, &(bModel->_pbrTextureArrays[3]->GetSRV()));

				//�켱������, ConstantBuffer���� ���Ѵ�.
				assert(bBufferPairList->_instancedLightMapSetVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING);

				for (int i = 0; i < bBufferPairList->_instancedLightMapSetVec.size(); i++)
				{
					_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = bBufferPairList->_instancedLightMapSetVec.at(i);
				}

				_lightmapCBuffer->Update();

				//���� PS CB Bind����.
				_lightmapCBuffer->BindPS(6);

				//Vertex / Index Buffer Setting.
				//1st :  Vin1stStatic_Individual
				//2nd :	 Vin2ndAll_Individual
				//3rd :  Vin3rdStaticSkinned_Individual

				UINT strides[3] = { sizeof(LayoutDefine::Vin1stStatic_Individual), sizeof(LayoutDefine::Vin2ndAll_Individual), sizeof(LayoutDefine::Vin3rdInstanced_Individual) };
				UINT offsets[3] = { 0,0,0 };
				ID3D11Buffer* vbArray[3] = { bModel->_vertexBuffer , bModel->_secondVertexBuffer, bBufferPairList->_vb }; // �̷��� �� ���� ������Ʈ ���۸� ��� ���� �� ó�� �Ϸ�.

				//
				_DXStorage->_deviceContext->IASetVertexBuffers(0, 3, vbArray, strides, offsets);

				//Index Buffer Setting.
				_DXStorage->_deviceContext->IASetIndexBuffer(bModel->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

				//�׸���.
				//DrawIndexedInstanced�� ���.
				int tMeshCount = bModel->_assetSceneData->_totalMeshCount;
				for (int i = 0; i < tMeshCount; i++)
				{
					//MultiMesh -> Material ������ �� �ְ� ���⼭�� Vector Clear.
					UINT tToDrawIndexCount = bModel->_assetSceneData->_meshList[i]._numIndices;

					//������Ʈ�� ������ ȣ��� �ش� Mesh��ŭ �׸���.
					_DXStorage->_deviceContext->DrawIndexedInstanced(tToDrawIndexCount, bBufferPairList->_instancedStaticPairVec.size(),
						bModel->_assetSceneData->_meshList[i]._indexOffset,
						bModel->_assetSceneData->_meshList[i]._vertexOffset, 0);
				}

				_lightmapCBuffer->UnbindPS(6);

				ID3D11ShaderResourceView* tNullSRV = nullptr;
				// Albedo
				_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &tNullSRV);
				//// Normal
				//_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &tNullSRV);
				//// Arm
				//_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &tNullSRV);

			}

			///���� VertexBuffer�� ������ Binding�Ǿ� ���� �� ��Ȳ����, ViewProj Switching. ���� �ٲ���.
			_switchableViewProjCBuffer->UnbindVS(1);
			//_switchableViewProjCBuffer->GetDataStruct()->_viewProj = _d3dCarrierTempStorage->_mainLightPerspectiveViewProjMatrix;
			//_switchableViewProjCBuffer->Update();
			//_switchableViewProjCBuffer->BindVS(1);
			//_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_shadowMapViewport));

			//Light ���� ����.
			{
				//// Unbind RenderTarget
				//_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);
				//
				////Shadow ���� ���� ����Ī.
				////_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
				////_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_d3dCarrierTempStorage->_mainLightGBufRT->GetRTV()), _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
				//ID3D11RenderTargetView* tEmptyRenderTargets[1] = { _d3dCarrierTempStorage->_mainLightGBufRT->GetRTV() };
				//_DXStorage->_deviceContext->OMSetRenderTargets(1, tEmptyRenderTargets, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());

				_alphaClippedPS->Unbind();
				//_alphaClippedDepthRecordOnlyPS->Bind();
				//
				//int tMeshCount = bModel->_assetSceneData->_totalMeshCount;
				//for (int i = 0; i < tMeshCount; i++)
				//{
				//	//MultiMesh -> Material ������ �� �ְ� ���⼭�� Vector Clear.
				//	UINT tToDrawIndexCount = bModel->_assetSceneData->_meshList[i]._numIndices;
				//
				//	//������Ʈ�� ������ ȣ��� �ش� Mesh��ŭ �׸���.
				//	_DXStorage->_deviceContext->DrawIndexedInstanced(tToDrawIndexCount, bBufferPairList->_instancedStaticPairVec.size(),
				//		bModel->_assetSceneData->_meshList[i]._indexOffset,
				//		bModel->_assetSceneData->_meshList[i]._vertexOffset, 0);
				//}
				//
				//_switchableViewProjCBuffer->UnbindVS(1);
				//_alphaClippedDepthRecordOnlyPS->Unbind();
				_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
				//��ϵ� �� ������, ������ ���� �غ�.

				//PSSetShaderResources : Alpha������ ����, �� ����..
				ID3D11ShaderResourceView* tNullSRV = nullptr;

				// Alpha
				_DXStorage->_deviceContext->PSSetShaderResources(11, 1, &tNullSRV);
			}

		}
	}

	void FirstInstancedRenderPass::RenderAlphaClippedCulledOppositeInstanced(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = reinterpret_cast<RenderObject3DList*>(renderObjectList);

		//�����ϱ� ����, Rasterizer State Cull �ݴ�� ������ �ȴ�. ��� �ݴ�� �ø�ȳ�� �ϴ� ģ���̴�.
		_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidNoCullingState);
		//_DXStorage->_deviceContext->RSSetState(_DXStorage->_solidNoCullingState);

		for (auto& [bModel, bBufferPairList] : tRenderObjectList->_instancedCulledOppositeStaticAlphaClippedList)
		{
			//���� �����Ұ� ����ִٸ�, continue.
			if (bBufferPairList->_instancedStaticPairVec.empty())
			{
				continue;
			}

			_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_defaultViewport));
			_switchableViewProjCBuffer->GetDataStruct()->_viewProj = Pg::Math::PG2XM_MATRIX4X4(camData->_viewMatrix * camData->_projMatrix);
			_switchableViewProjCBuffer->Update();
			_switchableViewProjCBuffer->BindVS(1);

			//Camera ���� ����.
			{
				//OMSetRenderTargets - ���� ��Ҿ�� �� ���� ���!
				_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(),
					_d3dCarrierTempStorage->_gBufRequiredRTVArray.data(), _d3dCarrierTempStorage->_gBufRequiredInfoDSV->GetDSV());

				//���� VS(Camera-Based) / PS ���ε�. 
				_alphaClippedPS->Bind();

				// Albedo
				_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &(bModel->_pbrTextureArrays[0]->GetSRV()));
				//// Normal 
				//_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &(bModel->_pbrTextureArrays[1]->GetSRV()));
				//// ARM
				//_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &(bModel->_pbrTextureArrays[2]->GetSRV()));
				// Alpha
				_DXStorage->_deviceContext->PSSetShaderResources(11, 1, &(bModel->_pbrTextureArrays[3]->GetSRV()));

				//�켱������, ConstantBuffer���� ���Ѵ�.
				assert(bBufferPairList->_instancedLightMapSetVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING);

				for (int i = 0; i < bBufferPairList->_instancedLightMapSetVec.size(); i++)
				{
					_lightmapCBuffer->GetDataStruct()->gBuf_LightMapSet[i] = bBufferPairList->_instancedLightMapSetVec.at(i);
				}

				_lightmapCBuffer->Update();

				//���� PS CB Bind����.
				_lightmapCBuffer->BindPS(6);

				//Vertex / Index Buffer Setting.
				//1st :  Vin1stStatic_Individual
				//2nd :	 Vin2ndAll_Individual
				//3rd :  Vin3rdStaticSkinned_Individual

				UINT strides[3] = { sizeof(LayoutDefine::Vin1stStatic_Individual), sizeof(LayoutDefine::Vin2ndAll_Individual), sizeof(LayoutDefine::Vin3rdInstanced_Individual) };
				UINT offsets[3] = { 0,0,0 };
				ID3D11Buffer* vbArray[3] = { bModel->_vertexBuffer , bModel->_secondVertexBuffer, bBufferPairList->_vb }; // �̷��� �� ���� ������Ʈ ���۸� ��� ���� �� ó�� �Ϸ�.

				//
				_DXStorage->_deviceContext->IASetVertexBuffers(0, 3, vbArray, strides, offsets);

				//Index Buffer Setting.
				_DXStorage->_deviceContext->IASetIndexBuffer(bModel->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

				//�׸���.
				//DrawIndexedInstanced�� ���.
				int tMeshCount = bModel->_assetSceneData->_totalMeshCount;
				for (int i = 0; i < tMeshCount; i++)
				{
					//MultiMesh -> Material ������ �� �ְ� ���⼭�� Vector Clear.
					UINT tToDrawIndexCount = bModel->_assetSceneData->_meshList[i]._numIndices;

					//������Ʈ�� ������ ȣ��� �ش� Mesh��ŭ �׸���.
					_DXStorage->_deviceContext->DrawIndexedInstanced(tToDrawIndexCount, bBufferPairList->_instancedStaticPairVec.size(),
						bModel->_assetSceneData->_meshList[i]._indexOffset,
						bModel->_assetSceneData->_meshList[i]._vertexOffset, 0);
				}

				_lightmapCBuffer->UnbindPS(6);

				ID3D11ShaderResourceView* tNullSRV = nullptr;
				// Albedo
				_DXStorage->_deviceContext->PSSetShaderResources(8, 1, &tNullSRV);
				//// Normal
				//_DXStorage->_deviceContext->PSSetShaderResources(9, 1, &tNullSRV);
				//// ARM
				//_DXStorage->_deviceContext->PSSetShaderResources(10, 1, &tNullSRV);
			}

			///���� VertexBuffer�� ������ Binding�Ǿ� ���� �� ��Ȳ����, ViewProj Switching. ���� �ٲ���.
			_switchableViewProjCBuffer->UnbindVS(1);
			//_switchableViewProjCBuffer->GetDataStruct()->_viewProj = _d3dCarrierTempStorage->_mainLightPerspectiveViewProjMatrix;
			//_switchableViewProjCBuffer->Update();
			//_switchableViewProjCBuffer->BindVS(1);
			//_DXStorage->_deviceContext->RSSetViewports(1, &(_DXStorage->_shadowMapViewport));

			//Light ���� ����.
			{
				// Unbind RenderTarget
				//_DXStorage->_deviceContext->OMSetRenderTargets(_d3dCarrierTempStorage->_gBufRequiredRTVArray.size(), _d3dCarrierTempStorage->NullRTV.data(), nullptr);
				//
				////Shadow ���� ���� ����Ī.
				////_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
				////_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_d3dCarrierTempStorage->_mainLightGBufRT->GetRTV()), _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
				//ID3D11RenderTargetView* tEmptyRenderTargets[1] = { _d3dCarrierTempStorage->_mainLightGBufRT->GetRTV() };
				//_DXStorage->_deviceContext->OMSetRenderTargets(1, tEmptyRenderTargets, _d3dCarrierTempStorage->_mainLightGBufDSV->GetDSV());
				
				_alphaClippedPS->Unbind();
				//_alphaClippedDepthRecordOnlyPS->Bind();

				//int tMeshCount = bModel->_assetSceneData->_totalMeshCount;
				//for (int i = 0; i < tMeshCount; i++)
				//{
				//	//MultiMesh -> Material ������ �� �ְ� ���⼭�� Vector Clear.
				//	UINT tToDrawIndexCount = bModel->_assetSceneData->_meshList[i]._numIndices;
				//
				//	//������Ʈ�� ������ ȣ��� �ش� Mesh��ŭ �׸���.
				//	_DXStorage->_deviceContext->DrawIndexedInstanced(tToDrawIndexCount, bBufferPairList->_instancedStaticPairVec.size(),
				//		bModel->_assetSceneData->_meshList[i]._indexOffset,
				//		bModel->_assetSceneData->_meshList[i]._vertexOffset, 0);
				//}
				//
				//_switchableViewProjCBuffer->UnbindVS(1);
				//_alphaClippedDepthRecordOnlyPS->Unbind();
				_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
				//��ϵ� �� ������, ������ ���� �غ�.

				//PSSetShaderResources : Alpha������ ����, �� ����..
				ID3D11ShaderResourceView* tNullSRV = nullptr;

				// Alpha
				_DXStorage->_deviceContext->PSSetShaderResources(11, 1, &tNullSRV);
			}

		}
	}

}
