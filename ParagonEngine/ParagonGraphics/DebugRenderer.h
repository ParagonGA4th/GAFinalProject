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
struct ID3D11DepthStencilState;

namespace Pg::Data
{
	class CameraData;
}

namespace Pg::Util
{
	namespace Time
	{
		class TimeSystem;
	}
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

		virtual void RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData) override;
		virtual void ConfirmCarrierData() override;

		void GetDebugBoxGeometryData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec);
		void GetDebugSphereGeometryData(const std::vector<Pg::Data::SphereInfo*>& const sphereColVec);
		void GetDebugCapsuleGeometryData(const std::vector<Pg::Data::CapsuleInfo*>& const capsuleColVec);
		void GetDebugLineGeometryData(const std::vector<Pg::Data::LineInfo>& const lineColVec);
		void GetDebugPlaneGeometryData(const std::vector<Pg::Data::PlaneInfo*>& const planeColVec);
		void GetDebugRayCastGeometryData(const std::vector<Pg::Data::RayCastInfo>& const rayCastColVec);
		void GetDebugBox2dGeometryData(const std::vector<Pg::Data::Box2DInfo>& const box2DColVec);
		void GetDebugNavMeshGeometryData(const std::vector<Pg::Data::NavMeshInfo*>& const navMeshVec);
		void GetDebugNavCylinderGeometryData(const std::vector<Pg::Data::NavCylinderInfo*>& const navCylinderVec);

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

		void BeginDebug2dRender(Pg::Data::CameraData* camData);
		void Debug2dRender();
		void EndDebug2dRender();


	private:
		void DrawBox(Pg::Data::CameraData* camData, Pg::Data::BoxInfo* boxInfo);
		void DrawSphere(Pg::Data::CameraData* camData, Pg::Data::SphereInfo* sphereInfo);
		void DrawCapsule(Pg::Data::CameraData* camData, Pg::Data::CapsuleInfo* capsuleInfo);
		void DrawLine(Pg::Data::LineInfo* lineInfo);
		void DrawRayCast(Pg::Data::RayCastInfo rayCastInfo);
		void DrawPlane(Pg::Data::CameraData* camData, Pg::Data::PlaneInfo* planeInfo);
		void DrawBox2D(Pg::Data::Box2DInfo box2dInfo);
		void DrawNavMesh(Pg::Data::CameraData* camData, Pg::Data::NavMeshInfo* navInfo);
		void DrawNavCylinder_ActualCylinder(Pg::Data::CameraData* camData, Pg::Data::NavCylinderInfo* cylInfo);
		void DrawNavCylinder_Lines(Pg::Data::NavCylinderInfo* cylInfo);
	private:
		void CreateSystemVertexShaders();
		void CreateDepthWriteOffDSS();
		void InitGeometry();
		void InitCapsule();
		void InitLine();
		void InitPlane();

	private:
		LowDX11Storage* _DXStorage;
		LowDX11Logic* _DXLogic;
		Pg::Util::Time::TimeSystem* _timeSystem{ nullptr };

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

		//NavMesh Cylinder Rendering.
		std::unique_ptr<DirectX::GeometricPrimitive> _cylinderShape;

		//Capsule Wireframe Rendering
		std::unique_ptr<DirectX::GeometricPrimitive> _capsuleShape;

		//Plane Wireframe Rendering
		std::unique_ptr<DirectX::GeometricPrimitive> _planeShape;

		//Nav Mesh Primitive Vector.
		std::unordered_map<std::string, std::unique_ptr<DirectX::GeometricPrimitive>> _navMeshPrimitiveVector;
	private:
		//렌더링을 위해 Pointer를 보관.
		const std::vector<Pg::Data::BoxInfo*>* _boxColVector = nullptr;
		const std::vector<Pg::Data::SphereInfo*>* _sphereColVector = nullptr;
		const std::vector<Pg::Data::CapsuleInfo*>* _capsuleColVector = nullptr;
		const std::vector<Pg::Data::LineInfo>* _lineColVector = nullptr;
		const std::vector<Pg::Data::PlaneInfo*>* _planeColVector = nullptr;
		const std::vector<Pg::Data::RayCastInfo>* _rayCastColVector = nullptr;
		const std::vector<Pg::Data::Box2DInfo>* _box2dVector = nullptr;
		const std::vector<Pg::Data::NavMeshInfo*>* _navMeshVector = nullptr;
		const std::vector<Pg::Data::NavCylinderInfo*>* _navCylinderVector = nullptr;

	private:
		//DebugLine을 위한 요구사항.

		//Line Wireframe Rendering
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> _primitiveBatch;
		std::unique_ptr<DirectX::BasicEffect> _basicEffect = nullptr;
		ID3D11InputLayout* _debugLineInputLayout = nullptr;

		std::unique_ptr<DirectX::BasicEffect> _basicEffect2d = nullptr;

		std::unique_ptr<DirectX::BasicEffect> _navMeshEffect = nullptr;
		ID3D11InputLayout* _navMeshInputLayout = nullptr;

		std::unique_ptr<DirectX::BasicEffect> _navCylinderEffect = nullptr;
		ID3D11InputLayout* _navCylinderInputLayout = nullptr;

		//CommonState
		std::unique_ptr<DirectX::CommonStates> _commonStates;

	private:
		//DepthWrite를 끄기.
		ID3D11DepthStencilState* _depthWriteOffDSS{ nullptr };

		
	};
}

