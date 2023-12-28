#pragma once
#include "IRenderPass.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>

namespace Pg::Graphics
{
	class GBufferRender;
	class GBufferDepthStencil;
	class LowDX11Storage;
	class RenderVertexShader;
	class RenderPixelShader;
	class RenderMaterial; //하나의 RenderMaterial을 기준으로 Pass가 구분될 것. (객체 생성 단위)
}

/// <summary>
/// 이 RenderPass는 사용되는 하나하나의 Material당 늘어나며,
/// 소팅된 오브젝트를 기준으로 렌더링을 시행한다.
/// RenderPass가 오브젝트에 고유하지 않은 상수 버퍼 / SRV 할당을 도맡아서 한다.
/// </summary>
namespace Pg::Graphics
{
	class OpaqueQuadRenderPass : public IRenderPass
	{
	public:
		OpaqueQuadRenderPass(RenderMaterial* renderMat);
		~OpaqueQuadRenderPass();

		virtual void Initialize() override;
		virtual void BindPass() override;
		virtual void RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void SetupNextRequirements() override;

	private:
		void GenerateQuadBuffer();
		void BindVertexIndexBuffer();

	private:
		ID3D11Buffer* _quadVB;
		ID3D11Buffer* _quadIB;
		RenderMaterial* _renderMaterial; //1 OpaqueQuadRenderPass = 1 Material Used.
		//FilePath는 RenderMaterial 상위 GraphicsResource에 보관되어 있다.

	private:
		LowDX11Storage* _DXStorage;
	};
}


