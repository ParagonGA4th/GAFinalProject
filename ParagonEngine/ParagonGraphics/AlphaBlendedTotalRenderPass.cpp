#include "AlphaBlendedTotalRenderPass.h"

#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "SystemVertexShader.h"
#include "RenderTexture2D.h"
#include "RenderCubemap.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/ParagonDefines.h"

#include <algorithm>  

namespace Pg::Graphics
{

	AlphaBlendedTotalRenderPass::AlphaBlendedTotalRenderPass(const Pg::Data::Enums::eEditorMode* const editorMode) :
		IRenderSinglePass(), _deltaTimeStorage(0.f), _editorMode(editorMode), _DXStorage(LowDX11Storage::GetInstance())
	{

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

	}

	void AlphaBlendedTotalRenderPass::BindPass()
	{
		//사용하지 않음. -> Static / Skinned 섞여 쓰이는 Alpha 특성상, RenderPass 내부에 쓰일 것..
	}

	void AlphaBlendedTotalRenderPass::RenderPass(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = (RenderObject3DList*)renderObjectList;

		const RenderMaterial* tPreviousRenderMaterial{ nullptr };

		for (auto& it : tRenderObjectList->_allAlphaBlendedList)
		{
			if (tPreviousRenderMaterial != it->_renderMat)
			{
				//전과 다르니, 이때는 바인딩을 해야 할 것이다.
				///잠깐만, 근데 그 말은 어느 정도 현재 구조를 유지하기 위해서는 인스턴싱을 시작해야 한다는 말이다.
				///왜냐? 이미 만들어져 있는 리소스를 싹 다 버려야 하기 때문..?
				///여튼, 그렇게 해야 할 것이다.

			}

			if (it->_isSkinned)
			{
				//Skinned.

			}
			else
			{
				//Static.

			}

			tPreviousRenderMaterial = it->_renderMat;
		}
	}

	void AlphaBlendedTotalRenderPass::UnbindPass()
	{
		//사용하지 않음. -> Static / Skinned 섞여 쓰이는 Alpha 특성상, RenderPass 내부에 쓰일 것..
	}

	void AlphaBlendedTotalRenderPass::ExecuteNextRenderRequirements()
	{

	}

	void AlphaBlendedTotalRenderPass::PassNextRequirements(D3DCarrier& gCarrier)
	{

	}

}

