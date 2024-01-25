#pragma once
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "BaseSpecificRenderer.h"
#include "../ParagonData/DebugData.h"

#include <memory>
#include <vector>

//나중에는 Forward Declaration으로 옮겨야!
#include <dxtk/VertexTypes.h>
#include <dxtk/CommonStates.h>
#include <dxtk/GeometricPrimitive.h>
#include <dxtk/PrimitiveBatch.h>
#include <dxtk/Effects.h>

/// 게임엔진에서 콜라이더 등 
/// 디버그 렌더 과정을 담당한다.

struct ID3D11InputLayout;

namespace Pg::Data
{
	class CameraData;
}

namespace Pg::Graphics
{
	class LowDX11Storage;
	class LowDX11Logic;
	class RenderObjectWireframeList;

	class DebugRenderer : public BaseSpecificRenderer
	{
	public:
		DebugRenderer(D3DCarrier* d3dCarrier);

	public:
		virtual void Initialize() override;
		virtual void SetupRenderPasses() override;

		virtual void RenderContents(void* renderObjectList, Pg::Data::CameraData* camData) override;
		virtual void ConfirmCarrierData() override;

		void GetDebugBoxGeometryData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec);
		void GetDebugSphereGeometryData(const std::vector<Pg::Data::SphereInfo*>& const sphereColVec);
		void GetDebugCapsuleGeometryData(const std::vector<Pg::Data::CapsuleInfo*>& const capsuleColVec);
		void GetDebugLineGeometryData(const std::vector<Pg::Data::LineInfo>& const lineColVec);
		void GetDebugPlaneGeometryData(const std::vector<Pg::Data::PlaneInfo*>& const planeColVec);
		void GetDebugRayCastGeometryData(const std::vector<Pg::Data::RayCastInfo>& const rayCastColVec);
		
		
	private:

		void Render(RenderObjectWireframeList* wireframeList, Pg::Data::CameraData* camData);
		//내부적으로 Rendering 세팅 설정.
		void WireframeObjRender(RenderObjectWireframeList* wireframeList, Pg::Data::CameraData* camData);

		void BeginGeoPrimitiveRender();
		void GeoPrimitiveRender(Pg::Data::CameraData* camData);
		void EndGeoPrimitiveRender();

		void BeginPrimitiveBatchRender(Pg::Data::CameraData* camData);
		void LineRender();
		void EndPrimitiveBatchRender();

	private:
		void DrawBox(Pg::Data::CameraData* camData, Pg::Data::BoxInfo* boxInfo);
		void DrawSphere(Pg::Data::CameraData* camData, Pg::Data::SphereInfo* sphereInfo);
		void DrawCapsule(Pg::Data::CameraData* camData, Pg::Data::CapsuleInfo* capsuleInfo);
		void DrawLine(Pg::Data::LineInfo* lineInfo);
		void DrawRayCast(Pg::Data::RayCastInfo rayCastInfo);
		void DrawPlane(Pg::Data::CameraData* camData, Pg::Data::PlaneInfo* planeInfo);

	private:
		void CreateSystemVertexShaders();
		void InitGeometry();
		void InitCapsule();
		void InitLine();
		void InitPlane();

	private:
		LowDX11Storage* _DXStorage;
		LowDX11Logic* _DXLogic;

	private:
		//SystemVertexShader : Primitive
		std::unique_ptr<SystemVertexShader> _primitiveVS;
		std::unique_ptr<SystemPixelShader> _primitivePS;

		//Wireframe Primitive RenderObjects

	private:
		//Box Wireframe Rendering
		std::unique_ptr<DirectX::GeometricPrimitive> _boxShape;
		//Sphere Wireframe Rendering
		std::unique_ptr<DirectX::GeometricPrimitive> _sphereShape;


		//Capsule Wireframe Rendering
		std::unique_ptr<DirectX::GeometricPrimitive> _capsuleShape;
		
		//Plane Wireframe Rendering
		std::unique_ptr<DirectX::GeometricPrimitive> _planeShape;
		

	private:
		//렌더링을 위해 Pointer를 보관.
		const std::vector<Pg::Data::BoxInfo*>* _boxColVector = nullptr;
		const std::vector<Pg::Data::SphereInfo*>* _sphereColVector = nullptr;
		const std::vector<Pg::Data::CapsuleInfo*>* _capsuleColVector = nullptr;
		const std::vector<Pg::Data::LineInfo>* _lineColVector = nullptr;
		const std::vector<Pg::Data::PlaneInfo*>* _planeColVector = nullptr;
		const std::vector<Pg::Data::RayCastInfo>* _rayCastColVector = nullptr;

	private:
		//DebugLine을 위한 요구사항.
	
		//Line Wireframe Rendering
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> _primitiveBatch;
		ID3D11InputLayout* _debugLineInputLayout = nullptr;
		std::unique_ptr<DirectX::BasicEffect> _basicEffect = nullptr;

		//CommonState
		std::unique_ptr<DirectX::CommonStates> _commonStates;
	};
}

