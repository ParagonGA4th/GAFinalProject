#pragma once

#include "DX11Headers.h"

#include <vector>
#include <memory>

namespace Pg::Data
{
	class GameObject;
	class CameraData;
}

namespace Pg::Graphics
{
	class TestCube;
	class LowDX11Storage;
	class GBuffer;
	class VertexShader;
	class PixelShader;
}

namespace Pg::Graphics
{
	class DeferredRenderer
	{
	public:
		DeferredRenderer();

	public:
		void Initialize();

		void BeginRender();

		void RenderFirstPass(Pg::Data::GameObject* object, Pg::Data::CameraData& camData);
		void RenderSecondPass();

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