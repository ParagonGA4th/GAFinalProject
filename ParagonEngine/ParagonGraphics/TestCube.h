#pragma once
//
//#include "WireframeRenderObject.h"
//#include "ConstantBufferDefine.h"
//
//namespace Pg::Graphics
//{
//	struct BoxCBData
//	{
//		DirectX::XMFLOAT4X4 worldMatrix;
//		DirectX::XMFLOAT4X4 viewMatrix;
//		DirectX::XMFLOAT4X4 projectionMatrix;
//		DirectX::XMFLOAT4X4 viewProjMatrix;
//		DirectX::XMFLOAT3 eyePos;
//	};
//}
//
//namespace Pg::Data
//{
//	class Transform;
//	class CameraData;
//}
//
//namespace Pg::Graphics
//{
//
//	class TestCube : public RenderableObject
//	{
//	public:
//		TestCube();
//
//	public:
//		virtual void Render() override;
//		virtual void Render(Pg::Data::Transform& transform, Pg::Data::CameraData& camData) override;
//
//		virtual void BuildBuffers() override;
//		virtual void BindBuffers() override;
//
//	private:
//
//		struct TestCubeVertex
//		{
//			float3 position;
//			float3 color;
//		};
//
//	public:
//		ConstantBufferDefine::cbPerObjectBase _cbData;
//	};
//}
