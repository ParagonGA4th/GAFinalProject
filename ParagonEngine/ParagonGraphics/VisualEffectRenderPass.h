#pragma once
#include "IRenderSinglePass.h"
#include "DX11Headers.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include "VisualEffectRenderer.h"

/// <summary>
/// Final Render Pass : Quad������ ���� MainRenderTarget�� �ѷ��ش�.
/// ��� �������� �����ؼ� �������� ������ ����� �� ��.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;
	class SystemVertexShader;
	class SystemPixelShader;
	class D3DCarrier;
}

/// <summary>
/// SpriteBatch�� �����ؼ� �پ��� ȿ�� (Alpha, 3D, ��Ÿ)
/// ����� ������ �÷ο쿡 ���߾� �������ִ� �н�.
/// �Ź� Camera�� �ٶ󺸴� Quad���, 
/// �׷����� �ʴ� Quad���� ������ �����Ǿ�� �Ѵ�.
/// �̴� �پ��� SpriteBatch�� �����ϴ� ������ ����.
/// Effectó�� ȣ��Ǿ�� �ϴ� �ֵ鵵 �ִ�.
/// ���������� ��ġ�� ���� ���� ������Ʈ�� �ƴϾ����� ���ڴ�.
/// �� ����, CSV�� ����� �޾Ƽ� �����ϰ� �ϱ�? 
/// void*�� �̸� �ް� �ϰ�,
/// �׸� ������� ������ �� �ְ�?
/// Vertex Buffer�� ���� ���� ������ ����. ������ Quad�̴�.
/// �ٸ�, �̸� � Ʈ������ + ȿ�� + ���̴� ��� ���̷� �ٸ��� ����������
/// ũ�� �߿��ϰ� �ۿ��� ��.
/// RenderPass����, ȿ�� ��� ���. CSV �Ľ� ����. 
/// �״����� Runtime ã�� ����� ���� �� �ְ�,
/// void*�� Effect�� RenderingAPI���� ȸ���� ������, 
/// �̸� �ܼ� ĳ�������� Ȱ���� �� �ְ� ��������.
/// Get�Ѵ����� ����, �̷� ����.
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
		//Effect Registering�� CSV�� ���ؼ� �̷������.
		void RegisterAllEffects();
		//Renderer ����, PgGraphics�� ����.
		VisualEffectRenderer* GetVisualEffectRenderer() { return _visualEffectController.get(); }

	private:
		std::unique_ptr<VisualEffectRenderer> _visualEffectController{ nullptr };
		

	private:
		LowDX11Storage* _DXStorage;

		std::string _resourceListPath;
		const D3DCarrier* _carrier{ nullptr };
	};
}
