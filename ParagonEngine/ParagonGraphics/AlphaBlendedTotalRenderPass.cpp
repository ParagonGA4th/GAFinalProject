#include "AlphaBlendedTotalRenderPass.h"

#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "RenderTexture2D.h"
#include "RenderCubemap.h"
#include "SystemVertexShader.h"
#include "D3DCarrier.h"
#include "LayoutDefine.h"
#include "SystemPixelShader.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/ParagonDefines.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/SkinnedMeshRenderer.h"

#include <algorithm>  
#include <functional>
#include <numbers>

namespace Pg::Graphics
{
	AlphaBlendedTotalRenderPass::AlphaBlendedTotalRenderPass(const Pg::Data::Enums::eEditorMode* const editorMode) :
		IRenderSinglePass(), _deltaTimeStorage(0.f), _editorMode(editorMode), _DXStorage(LowDX11Storage::GetInstance())
	{
		_commonStates = std::make_unique<DirectX::CommonStates>(_DXStorage->_device);
		_cbAlpha = std::make_unique<ConstantBuffer<ConstantBufferDefine::cbAlphaDefault>>();
	}

	AlphaBlendedTotalRenderPass::~AlphaBlendedTotalRenderPass()
	{

	}

	void AlphaBlendedTotalRenderPass::Initialize()
	{

	}

	void AlphaBlendedTotalRenderPass::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

	void AlphaBlendedTotalRenderPass::ReceiveRequiredElements(const D3DCarrier& carrier)
	{
		this->_carrier = &carrier;
	}

	void AlphaBlendedTotalRenderPass::BindPass()
	{
		//사용하지 않음. -> Static / Skinned 섞여 쓰이는 Alpha 특성상, RenderPass 내부에 쓰일 것..
		//Render Target 설정하기.

		//VS/PS는 RenderPass 내부에 세팅.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_carrier->_quadMainRT->GetRTV()), _carrier->_gBufRequiredInfoDSV->GetDSV());

		//이제 대망의 블렌드 스테이트를 저장한다!
		//_DXStorage->_deviceContext->OMSetBlendState(_commonStates->AlphaBlend(), NULL, 0xffffffff);
		_DXStorage->_deviceContext->OMSetBlendState(_commonStates->Additive(), NULL, 0xffffffff);
	}

	void AlphaBlendedTotalRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = (RenderObject3DList*)renderObjectList;

		//Blend하기.
		tRenderObjectList->SortBlendedByDepth_BackToFront(camData);

		RenderMaterial* tPreviousRenderMaterial{ nullptr };

		for (int i = 0; i < tRenderObjectList->_sortedIndexBlendedVec.size(); i++)
		{
			//이미 소팅된 Alpha Tuple을 받아온다.
			//이 부분에서 쓰일 Material은 : ForwardPS 렌더여야 한다.
			//Material에서 최종 렌더까지 한꺼번에!
			auto& bAlphaBlendedTuple = tRenderObjectList->_allAlphaBlendedList.at(tRenderObjectList->_sortedIndexBlendedVec.at(i));

			//Input Layout 업데이트.
			ID3D11InputLayout* tInputLayout{ nullptr };
			float tNormalizedAlpha{ 0.f };
			//InputLayout + Alpha (정규화된) : 기록.
			if (bAlphaBlendedTuple->_isSkinned)
			{
				tInputLayout = LayoutDefine::GetStatic1stLayout();
				tNormalizedAlpha = bAlphaBlendedTuple->_eitherSkinnedMesh->GetSkinnedMeshRenderer()->GetAlphaPercentage() / 100.f;
			}
			else
			{
				tInputLayout = LayoutDefine::GetSkinned1stLayout();
				tNormalizedAlpha = bAlphaBlendedTuple->_eitherStaticMesh->GetStaticMeshRenderer()->GetAlphaPercentage() / 100.f;
			}

			bool tIsDifferentBool = (tPreviousRenderMaterial != bAlphaBlendedTuple->_renderMat); //RenderMaterial이 다른지.
			if (tPreviousRenderMaterial != bAlphaBlendedTuple->_renderMat)
			{
				//전꺼 바인드 해제.
				if (tPreviousRenderMaterial != nullptr) { tPreviousRenderMaterial->Unbind(); }
				//변한 매터리얼 바인드.
				bAlphaBlendedTuple->_renderMat->Bind(tInputLayout);
			}

			//별개로 ConstantBuffer용 값 업데이트해야 함.
			//cbAlphaDefault Update + Bind.
			{
				_cbAlpha->GetDataStruct()->gCBuf_CameraViewProj = DirectX::XMMatrixMultiply(
					Pg::Math::PG2XM_MATRIX4X4(camData->_viewMatrix), Pg::Math::PG2XM_MATRIX4X4(camData->_projMatrix));
				_cbAlpha->GetDataStruct()->gCBuf_ObjectAlpha = tNormalizedAlpha;
				_cbAlpha->Update();

				_cbAlpha->BindVS(1);
				_cbAlpha->BindPS(1);
			}

			//Skinned / Not Skinned 기준 구분.
			if (bAlphaBlendedTuple->_isSkinned)
			{
				bAlphaBlendedTuple->_eitherSkinnedMesh->UpdateAnimationInfo(&_deltaTimeStorage, _editorMode);
				bAlphaBlendedTuple->_eitherSkinnedMesh->First_UpdateConstantBuffers(camData);
				bAlphaBlendedTuple->_eitherSkinnedMesh->First_BindBuffers();
				bAlphaBlendedTuple->_eitherSkinnedMesh->First_Render(&_deltaTimeStorage);
				bAlphaBlendedTuple->_eitherSkinnedMesh->First_UnbindBuffers();
			}
			else
			{
				bAlphaBlendedTuple->_eitherStaticMesh->First_UpdateConstantBuffers(camData);
				bAlphaBlendedTuple->_eitherStaticMesh->First_BindBuffers();
				bAlphaBlendedTuple->_eitherStaticMesh->First_Render(nullptr);
				bAlphaBlendedTuple->_eitherStaticMesh->First_UnbindBuffers();
			}

			//ConstantBuffer 해제.
			{
				_cbAlpha->UnbindVS(1);
				_cbAlpha->UnbindPS(1);
			}

			tPreviousRenderMaterial = bAlphaBlendedTuple->_renderMat;
		}

		//마지막으로 보내진 PreviousRenderMaterial Unbind.
		if (tPreviousRenderMaterial != nullptr)
		{
			tPreviousRenderMaterial->Unbind();
		}
	}

	void AlphaBlendedTotalRenderPass::UnbindPass()
	{
		//BlendState Reset.
		_DXStorage->_deviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);

		//사용하지 않음. -> Static / Skinned 섞여 쓰이는 Alpha 특성상, RenderPass 내부에 쓰일 것..
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}

	void AlphaBlendedTotalRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void AlphaBlendedTotalRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

}

