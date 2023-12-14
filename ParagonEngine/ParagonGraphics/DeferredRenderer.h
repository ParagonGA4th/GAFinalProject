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
	class RenderObject3DList;
	class RenderObjectLightList;
	class LowDX11Storage;
	class GBuffer;
	class SystemVertexShader;
	class SystemPixelShader;
	class ConstantBufferBase;
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

		void RenderFirstPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData);
		void RenderLight(RenderObjectLightList* lightList, Pg::Data::CameraData* camData);
		void RenderSecondPass();

		void ClearGBuffers();
	
		void BindFirstPass();
		void BindLightingPass();
		void BindSecondPass();

		void UnbindFirstPass();
		void UnbindLightingPass();
		void UnbindSecondPass();
	private:

		void BuildFullscreenQuad();
		void BindFullscreenQuad();
	private:
		std::vector<GBuffer*> _gBuffers;
		std::vector<ID3D11RenderTargetView*> _RTVs;
		std::vector<ID3D11ShaderResourceView*> _SRVs;

		std::vector<ID3D11RenderTargetView*> NullRTV;
		std::vector<ID3D11ShaderResourceView*> NullSRV;

	private:
		SystemVertexShader* _firstVS;
		SystemPixelShader* _firstPS;

		SystemVertexShader* _lightingVS;
		SystemPixelShader* _lightingPS;

		SystemVertexShader* _secondVS;
		SystemPixelShader* _secondPS;

	private:
		ID3D11Buffer* _VB;
		ID3D11Buffer* _IB;

	public:
		std::vector<ConstantBufferBase*> _firstCBs;
		std::vector<ConstantBufferBase*> _lightingCBs;
		std::vector<ConstantBufferBase*> _secondCBs;

	private:
		void UpdateConstantBuffers(const std::vector< ConstantBufferBase*>& _constantBuffers);
		void BindConstantBuffers(const std::vector< ConstantBufferBase*>& _constantBuffers);
		void UnbindConstantBuffers(const std::vector< ConstantBufferBase*>& _constantBuffers);

	private:
		LowDX11Storage* _DXStorage;

	private:
		TestCube* cube;
	};
}