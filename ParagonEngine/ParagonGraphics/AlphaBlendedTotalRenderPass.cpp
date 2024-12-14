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
		//������� ����. -> Static / Skinned ���� ���̴� Alpha Ư����, RenderPass ���ο� ���� ��..
		//Render Target �����ϱ�.

		//VS/PS�� RenderPass ���ο� ����.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_carrier->_quadMainRT->GetRTV()), _carrier->_gBufRequiredInfoDSV->GetDSV());

		//���� ����� ���� ������Ʈ�� �����Ѵ�!
		//_DXStorage->_deviceContext->OMSetBlendState(_commonStates->AlphaBlend(), NULL, 0xffffffff);
		_DXStorage->_deviceContext->OMSetBlendState(_commonStates->Additive(), NULL, 0xffffffff);
	}

	void AlphaBlendedTotalRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = (RenderObject3DList*)renderObjectList;

		//Blend�ϱ�.
		tRenderObjectList->SortBlendedByDepth_BackToFront(camData);

		RenderMaterial* tPreviousRenderMaterial{ nullptr };

		for (int i = 0; i < tRenderObjectList->_sortedIndexBlendedVec.size(); i++)
		{
			//�̹� ���õ� Alpha Tuple�� �޾ƿ´�.
			//�� �κп��� ���� Material�� : ForwardPS �������� �Ѵ�.
			//Material���� ���� �������� �Ѳ�����!
			auto& bAlphaBlendedTuple = tRenderObjectList->_allAlphaBlendedList.at(tRenderObjectList->_sortedIndexBlendedVec.at(i));

			//Input Layout ������Ʈ.
			ID3D11InputLayout* tInputLayout{ nullptr };
			float tNormalizedAlpha{ 0.f };
			//InputLayout + Alpha (����ȭ��) : ���.
			if (bAlphaBlendedTuple->_isSkinned)
			{
				tInputLayout = LayoutDefine::GetSkinned1stLayout();
				tNormalizedAlpha = bAlphaBlendedTuple->_eitherSkinnedMesh->GetSkinnedMeshRenderer()->GetAlphaPercentage() / 100.f;
			}
			else
			{
				tInputLayout = LayoutDefine::GetStatic1stLayout();
				tNormalizedAlpha = bAlphaBlendedTuple->_eitherStaticMesh->GetStaticMeshRenderer()->GetAlphaPercentage() / 100.f;
			}

			bool tIsDifferentBool = (tPreviousRenderMaterial != bAlphaBlendedTuple->_renderMat); //RenderMaterial�� �ٸ���.
			if (tPreviousRenderMaterial != bAlphaBlendedTuple->_renderMat)
			{
				//���� ���ε� ����.
				if (tPreviousRenderMaterial != nullptr) { tPreviousRenderMaterial->Unbind(); }
				//���� ���͸��� ���ε�.
				bAlphaBlendedTuple->_renderMat->Bind(tInputLayout);
			}

			//������ ConstantBuffer�� �� ������Ʈ�ؾ� ��.
			//cbAlphaDefault Update + Bind.
			{
				_cbAlpha->GetDataStruct()->gCBuf_CameraViewProj = DirectX::XMMatrixMultiply(
					Pg::Math::PG2XM_MATRIX4X4(camData->_viewMatrix), Pg::Math::PG2XM_MATRIX4X4(camData->_projMatrix));
				_cbAlpha->GetDataStruct()->gCBuf_ObjectAlpha = tNormalizedAlpha;
				_cbAlpha->Update();

				_cbAlpha->BindVS(1);
				_cbAlpha->BindPS(1);
			}

			//Skinned / Not Skinned ���� ����.
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

			//ConstantBuffer ����.
			{
				_cbAlpha->UnbindVS(1);
				_cbAlpha->UnbindPS(1);
			}

			tPreviousRenderMaterial = bAlphaBlendedTuple->_renderMat;
		}

		//���������� ������ PreviousRenderMaterial Unbind.
		if (tPreviousRenderMaterial != nullptr)
		{
			tPreviousRenderMaterial->Unbind();
		}
	}

	void AlphaBlendedTotalRenderPass::UnbindPass()
	{
		//BlendState Reset.
		_DXStorage->_deviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);

		//������� ����. -> Static / Skinned ���� ���̴� Alpha Ư����, RenderPass ���ο� ���� ��..
		_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}

	void AlphaBlendedTotalRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void AlphaBlendedTotalRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

}

