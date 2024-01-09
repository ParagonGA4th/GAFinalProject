#pragma once
#include "RenderObject3D.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"
#include <memory>

/// <summary>
/// StaticMeshRenderer 컴포넌트와 1대1 대응하는 렌더오브젝트.
/// </summary>

namespace Pg::Data
{
	class StaticMeshRenderer;
}

namespace Pg::Graphics
{
	class RenderObjectStaticMesh3D : public RenderObject3D
	{
	public:
		RenderObjectStaticMesh3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID, unsigned int matID);
		virtual ~RenderObjectStaticMesh3D();
		
		//Object-Material 데이터가 전부 매칭/로드 된 후, 일괄적으로 발동될 함수이다.	
		virtual void CreateObjMatBuffers() override;
	public:
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void First_BindBuffers() override;
		virtual void First_Render() override;
		virtual void First_UnbindBuffers() override;

		virtual void ObjMat_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void ObjMat_BindBuffers() override;
		virtual void ObjMat_Render() override;
		virtual void ObjMat_UnbindBuffers() override;

	private:
		//후에 셰이더 라이팅에 쓰일 메인 Vertex / Index Buffer 바인딩.
		void BindMainVertexIndexBuffer();
		//ObjMat 판단 위한 Vertex / Index Buffer 바인딩.
		void BindObjMatVertexIndexBuffer();

		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectBase>> _cbFirst;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjMatBase>> _cbObjMat;
	};
}



