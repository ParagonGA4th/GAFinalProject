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
	class RenderObject3D;
	class RenderObject3DList;
	class RenderObjectLightList;
	class LowDX11Storage;
	class GBuffer;
	class VertexShader;
	class PixelShader;
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
		VertexShader* _firstVS;
		PixelShader* _firstPS;

		VertexShader* _lightingVS;
		PixelShader* _lightingPS;
		
		VertexShader* _secondVS;
		PixelShader* _secondPS;

	private:
		ID3D11Buffer* _VB;
		ID3D11Buffer* _IB;

	public:
		std::vector< ConstantBufferBase* > _firstCBs;
		std::vector< ConstantBufferBase* > _lightingCBs;
		std::vector< ConstantBufferBase* > _secondCBs;

	private:
		void UpdateConstantBuffers(std::vector< ConstantBufferBase*> _constantBuffers);
		void BindConstantBuffers(std::vector< ConstantBufferBase*> _constantBuffers);
		void UnbindConstantBuffers(std::vector< ConstantBufferBase*> _constantBuffers);

	private:
		bool IsInFrustum(RenderObject3D* object, Pg::Data::CameraData* camData);

	private:
		LowDX11Storage* _DXStorage;

	private:
		TestCube* cube;
	};
}