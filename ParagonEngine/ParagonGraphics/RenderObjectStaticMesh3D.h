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
		RenderObjectStaticMesh3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID);
		virtual ~RenderObjectStaticMesh3D();
		
		//Constant Buffer로 전달.

	public:
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void First_BindBuffers() override;
		virtual void First_Render(const float* const dt) override;
		virtual void First_UnbindBuffers() override;
		
		Pg::Data::StaticMeshRenderer* GetStaticMeshRenderer() { return _staticMeshRenderer; }
	private:
		//후에 셰이더 라이팅에 쓰일 메인 Vertex / Index Buffer 바인딩.
		void BindMainVertexIndexBuffer();


	private:
		//내부 SkinnedMeshRenderer 캐스팅해서 보관하고 있다.
		Pg::Data::StaticMeshRenderer* _staticMeshRenderer{ nullptr };
	
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectBase>> _cbFirst;
	};
}



