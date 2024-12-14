#pragma once
#include "../ParagonData/GameConstantData.h"
#include <dxtk/PostProcess.h>
#include <memory>

namespace Pg::Graphics
{
	class LowDX11Storage;
	class D3DCarrier;

	class FadeInOutPass
	{
	public:
		FadeInOutPass();
		void Initialize();
		void Render(float baseShowRatio, D3DCarrier* d3dCarrier, ID3D11ShaderResourceView* sourceTexture);
	private:
		void CreateBlankTexture(ID3D11Device* pDevice, float width, float height);
	private:
		std::unique_ptr<DirectX::DualPostProcess> _dualPostProcess{ nullptr };
		ID3D11Texture2D* _blankTexture{ nullptr };
		ID3D11ShaderResourceView* _blankSRV{ nullptr };
	private:
		LowDX11Storage* _DXStorage;
	};
}
