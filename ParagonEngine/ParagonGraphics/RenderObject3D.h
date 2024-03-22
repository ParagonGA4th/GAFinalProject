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
		RenderObject3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID);
		virtual ~RenderObject3D();
		
		//일괄적으로 GraphicsParser에서 받을 수 있게, 설정.
		void SetMaterialID(unsigned int matID);

		//Object-Material 데이터가 전부 매칭/로드 된 후, 일괄적으로 발동될 함수이다.	
		virtual void CreateObjMatBuffers() abstract;

		unsigned int GetObjectID() const;
		unsigned int GetMaterialID() const;
	public:
		//FirstRenderPass에 쓰인다.
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) abstract;
		virtual void First_BindBuffers() abstract;
		virtual void First_Render(const float* const dt) abstract;
		virtual void First_UnbindBuffers() abstract;

		virtual void ObjMat_UpdateConstantBuffers(Pg::Data::CameraData* camData) abstract;
		virtual void ObjMat_BindBuffers() abstract;
		virtual void ObjMat_Render(const float* const dt) abstract;
		virtual void ObjMat_UnbindBuffers() abstract;

	protected:
		LowDX11Storage* _DXStorage;
		Asset3DModelData* _modelData = nullptr;
		
		//Model에 종속된 VB/IB와 다르게, Object, Material ID를 기록하기 위해 오브젝트 종속 VB들.
		ID3D11Buffer* _objMatVB = nullptr;
		//Index Buffer는 자신이 소속된 IndexBuffer와 동일.

	protected:
		//3D 오브젝트 한정.
		unsigned int _objectID;
		unsigned int _materialID;
	};
}