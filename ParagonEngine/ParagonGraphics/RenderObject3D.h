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
		
		//일괄적으로 GraphicsParser에서 받을 수 있게, 설정.
		//일괄적으로 Material ID Pointer를 부여해서, 나중에 연동되어도 문제 생기지 않게.
		void SetMaterialIdPointer(unsigned int* matID);

		//Object-Material 데이터가 전부 매칭/로드 된 후, 일괄적으로 발동될 함수이다.	
		virtual void CreateObjMatBuffers() {};

		unsigned int GetObjectID() const;
		unsigned int GetMaterialID() const;
	public:
		//FirstRenderPass에 쓰인다.
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) abstract;
		virtual void First_BindBuffers() abstract;
		virtual void First_Render(const float* const dt) abstract;
		virtual void First_UnbindBuffers() abstract;

		//Cull되면 렌더되지 않는 것.
		bool GetIsCulledFromRendering() { return _isCulledFromRendering; }
		void SetIsCulledFromRendering(bool val) { _isCulledFromRendering = val; }

	protected:
		LowDX11Storage* _DXStorage;
		Asset3DModelData* _modelData = nullptr;
		Pg::Data::RendererBase3D* _rendererBase3DStorage{ nullptr };
		//Static, Skinned는 Constant Buffer 쪽에서 가져왔고, 
		//Instanced는 사용 X.

	protected:
		//3D 오브젝트 한정.
		UINT _objectID;
		const UINT* _materialIdPointer;
		
		//현재 정보가 GameObject의 정보와 문제 없이 연동되었는가?
		//최신 정보인가/
		bool _isInternalUpToDate{ false };

	private:
		bool _isCulledFromRendering{ false }; //매 프레임마다 갱신됨. False면 렌더되는 것!
	};
}