#pragma once
#include "RenderObjectBase.h"

#include "ConstantBufferDefine.h"
#include "ConstantBuffer.h"

#include "DX11Headers.h"
#include <string>

/// <summary>
/// GameObject가 그래픽엔진의 렌더링 로직이랑 1대1 연동될 수 있게 하는 클래스이다.
/// 3D랑 호환된다.
/// 
/// Core에서 BaseRenderer가 붙은 GameObject들 :
/// RenderObject는 그래픽엔진에 독자적으로 관리되지 않는다.
/// </summary>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

namespace Pg::Data
{
	class Transform;
	class CameraData;
}

namespace Pg::Graphics
{
	class LowDX11Storage;
	class Asset3DModelData;
	class RenderVertexShader;
	class RenderPixelShader;
	class ConstantBufferDefine;
	struct ConstantBufferDefine::cbPerObjectBase;
	class RenderTexture2D;
	class RenderMaterial;
}

namespace Pg::Graphics
{
	class RenderObject3D : public Pg::Graphics::RenderObjectBase
	{
	public:
		RenderObject3D(Pg::Data::BaseRenderer* baseRenderer);
		virtual ~RenderObject3D();
			
		//FirstRenderPass에 쓰인다.
		virtual void UpdateConstantBuffers(Pg::Data::CameraData* camData) abstract;
		virtual void BindBuffers() abstract;
		virtual void Render() abstract;
		virtual void UnbindBuffers() abstract;

	protected:
		LowDX11Storage* _DXStorage;
		Asset3DModelData* _modelData = nullptr;
		
		//어떤 Material이랑 연동되었는지, 알 필요가 있다. -> 이미 RendererBase3D에 있다.
		//Parsing 과정에서 알아내야 하기 때문. (Sorting 때문에)
		//std::string _materialName;
	};
}