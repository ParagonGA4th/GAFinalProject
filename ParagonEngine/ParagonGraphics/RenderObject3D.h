#pragma once
#include "RenderObjectBase.h"

#include "ConstantBufferDefine.h"
#include "ConstantBuffer.h"

#include "DX11Headers.h"
#include <string>

/// <summary>
/// GameObject�� �׷��ȿ����� ������ �����̶� 1��1 ������ �� �ְ� �ϴ� Ŭ�����̴�.
/// 3D�� ȣȯ�ȴ�.
/// 
/// Core���� BaseRenderer�� ���� GameObject�� :
/// RenderObject�� �׷��ȿ����� ���������� �������� �ʴ´�.
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
	class RendererBase3D;
}

namespace Pg::Graphics
{
	class LowDX11Storage;
	class Asset3DModelData;
	class RenderVertexShader;
	class RenderPixelShader;
	class ConstantBufferDefine;
	class GraphicsSceneParser;
	struct ConstantBufferDefine::cbPerObjectBase;
	class RenderTexture2D;
	class RenderMaterial;
	class RenderObject3DList;
}

namespace Pg::Graphics
{
	class RenderObject3D : public Pg::Graphics::RenderObjectBase
	{
		friend class GraphicsSceneParser;
		friend class RenderObject3DList;
	public:
		RenderObject3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID);
		virtual ~RenderObject3D();
		
		//�ϰ������� GraphicsParser���� ���� �� �ְ�, ����.
		//�ϰ������� Material ID Pointer�� �ο��ؼ�, ���߿� �����Ǿ ���� ������ �ʰ�.
		void SetMaterialIdPointer(unsigned int* matID);

		//Object-Material �����Ͱ� ���� ��Ī/�ε� �� ��, �ϰ������� �ߵ��� �Լ��̴�.	
		virtual void CreateObjMatBuffers() {};

		unsigned int GetObjectID() const;
		unsigned int GetMaterialID() const;
	public:
		//FirstRenderPass�� ���δ�.
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) abstract;
		virtual void First_BindBuffers() abstract;
		virtual void First_Render(const float* const dt) abstract;
		virtual void First_UnbindBuffers() abstract;

		//Cull�Ǹ� �������� �ʴ� ��.
		bool GetIsCulledFromRendering() { return _isCulledFromRendering; }
		void SetIsCulledFromRendering(bool val) { _isCulledFromRendering = val; }

	protected:
		LowDX11Storage* _DXStorage;
		Asset3DModelData* _modelData = nullptr;
		Pg::Data::RendererBase3D* _rendererBase3DStorage{ nullptr };
		//Static, Skinned�� Constant Buffer �ʿ��� �����԰�, 
		//Instanced�� ��� X.

	protected:
		//3D ������Ʈ ����.
		UINT _objectID;
		const UINT* _materialIdPointer;
		
		//���� ������ GameObject�� ������ ���� ���� �����Ǿ��°�?
		//�ֽ� �����ΰ�/
		bool _isInternalUpToDate{ false };

	private:
		bool _isCulledFromRendering{ false }; //�� �����Ӹ��� ���ŵ�. False�� �����Ǵ� ��!
	};
}