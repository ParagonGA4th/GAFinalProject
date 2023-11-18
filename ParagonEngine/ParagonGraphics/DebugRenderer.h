#pragma once
#include <memory>

//나중에는 Forward Declaration으로 옮겨야!
#include <dxtk/VertexTypes.h>
#include <dxtk/CommonStates.h>
#include <dxtk/GeometricPrimitive.h>
#include <dxtk/PrimitiveBatch.h>

/// 게임엔진에서 콜라이더 등 
/// 디버그 렌더 과정을 담당한다.

//class DirectX::GeometricPrimitive;
//template<> class DirectX::PrimitiveBatch<DirectX::VertexPositionColor>;

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
		void BeginRender();
		void Render(Pg::Data::CameraData* camData);

	private:
		void CreateGeometry();

	private:
		LowDX11Storage* _DXStorage;
		LowDX11Logic* _DXLogic;

	private:
		//Box Wireframe Rendering
		std::unique_ptr<DirectX::GeometricPrimitive> _boxShape;
		//Sphere Wireframe Rendering
		std::unique_ptr<DirectX::GeometricPrimitive> _sphereShape;
		//Capsule Wireframe Rendering
		std::unique_ptr<DirectX::GeometricPrimitive> _capsuleShape;
		//Line Wireframe Rendering
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> _primitiveBatch;
	
		//CommonState
		std::unique_ptr<DirectX::CommonStates> _commonStates;
	};
}

