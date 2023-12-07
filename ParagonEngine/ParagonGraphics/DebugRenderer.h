#pragma once
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

	class DebugRenderer
	{
	public:
		DebugRenderer();

	public:
		void Initialize();
		void GetDebugBoxGeometryData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec);
		void Render(Pg::Data::CameraData* camData);

	private:
		//내부적으로 Rendering 세팅 설정.
		void BeginGeoPrimitiveRender();
		void GeoPrimitiveRender(Pg::Data::CameraData* camData);
		void EndGeoPrimitiveRender();

		void BeginPrimitiveBatchRender(Pg::Data::CameraData* camData);
		void LineRender();
		void EndPrimitiveBatchRender();

	private:
		void InitGeometry();
		void InitLine();

	private:
		LowDX11Storage* _DXStorage;
		LowDX11Logic* _DXLogic;

	private:
		//Box Wireframe Rendering
		std::unique_ptr<DirectX::GeometricPrimitive> _boxShape;
		//Sphere Wireframe Rendering
		std::unique_ptr<DirectX::GeometricPrimitive> _sphereShape;

		//Capsule Wireframe Rendering. (Sphere와 합쳐서 Capsule 구성에 활용)
		std::unique_ptr<DirectX::GeometricPrimitive> _topHemisphereShape;
		std::unique_ptr<DirectX::GeometricPrimitive> _cylinderShape;
		std::unique_ptr<DirectX::GeometricPrimitive> _bottomHemisphereShape;

	private:
		//렌더링을 위해 Pointer를 보관.
		const std::vector<Pg::Data::BoxInfo*>* _boxColVector = nullptr;

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

