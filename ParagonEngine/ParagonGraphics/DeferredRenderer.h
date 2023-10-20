#pragma once

#include <vector>
#include <memory>

#include "DX11Headers.h"

namespace Pg::Data
{
	class GameObject;
	class CameraData;
}

namespace Pg::Graphics
{
	class GBuffer;
	class TestCube;
	class VertexShader;
	class PixelShader;
	class LowDX11Storage;

	class DeferredRenderer
	{
	public:
		DeferredRenderer();

	public:
		void Initialize();

		// 각각의 GBuffer들에 렌더
		void Render(Pg::Data::GameObject& object, Pg::Data::CameraData& camData);

		void ClearGBuffers();
	private:
		void BindFirstPass();
		void BindSecondPass();

		void UnbindFirstPass();
		void UnbindSecondPass();

		void BuildFullscreenQuad();
	private:
		std::vector<GBuffer*> _gBuffers;
		std::vector<ID3D11RenderTargetView*> _RTVs;
		std::vector<ID3D11ShaderResourceView*> _SRVs;

		std::vector<ID3D11RenderTargetView*> NullRTV;
		std::vector<ID3D11ShaderResourceView*> NullSRV;

	private:
		VertexShader* _firstVS;
		PixelShader* _firstPS;
		
		VertexShader* _secondVS;
		PixelShader* _secondPS;

	private:
		LowDX11Storage* _DXStorage;

	private:
		TestCube* cube;
	};
}