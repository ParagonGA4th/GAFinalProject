#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include "VisualEffectRenderer.h"

/// <summary>
/// Final Render Pass : Quad에서의 값을 MainRenderTarget에 뿌려준다.
/// 모든 렌더링을 포함해서 최종적인 렌더링 결과가 될 것.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemPixelShader;
	class D3DCarrier;
}

/// <summary>
/// SpriteBatch를 가공해서 다양한 효과 (Alpha, 3D, 기타)
/// 등등을 게임의 플로우에 맞추어 렌더해주는 패스.
/// 매번 Camera를 바라보는 Quad들과, 
/// 그렇지는 않는 Quad들이 나뉘어 관리되어야 한다.
/// 이는 다양한 SpriteBatch를 지정하는 것으로 실행.
/// Effect처럼 호출되어야 하는 애들도 있다.
/// 개별적으로 위치를 갖는 게임 오브젝트가 아니었으면 좋겠다.
/// 얘 역시, CSV로 목록을 받아서 관리하게 하기? 
/// void*로 이름 받게 하고,
/// 그를 기반으로 실행할 수 있게?
/// Vertex Buffer가 여러 개일 이유는 없다. 어차피 Quad이니.
/// 다만, 이를 어떤 트랜스폼 + 효과 + 셰이더 등등 차이로 다르게 렌더할지가
/// 크게 중요하게 작용할 것.
/// RenderPass이자, 효과 출력 기반. CSV 파싱 역시. 
/// 그다음에 Runtime 찾는 비용을 줄일 수 있게,
/// void*로 Effect를 RenderingAPI에서 회수한 다음에, 
/// 이를 단순 캐스팅으로 활용할 수 있게 세팅하자.
/// Get한다음에 실행, 이런 느낌.
/// </summary>

namespace Pg::Graphics
{
	class VisualEffectRenderPass : public IRenderSinglePass
	{
	public:
		VisualEffectRenderPass(const std::string& resourceListPath);
		~VisualEffectRenderPass();

		virtual void Initialize() override;
		virtual void ReceiveRequiredElements(const D3DCarrier& carrier) override;
		virtual void BindPass() override;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void UnbindPass() override;
		virtual void ExecuteNextRenderRequirements() override;
		virtual void PassNextRequirements(D3DCarrier& gCarrier) override;
	
	public:
		//Effect Registering은 CSV를 통해서 이루어진다.
		void RegisterAllEffects();
		//Renderer 리턴, PgGraphics를 위해.
		VisualEffectRenderer* GetVisualEffectRenderer() { return _visualEffectController.get(); }

	private:
		std::unique_ptr<VisualEffectRenderer> _visualEffectController{ nullptr };
		

	private:
		LowDX11Storage* _DXStorage;

		std::string _resourceListPath;
		const D3DCarrier* _carrier{ nullptr };
	};
}
