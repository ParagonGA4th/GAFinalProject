#pragma once
#include "RenderObject3D.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"
#include <memory>


/// <summary>
/// StaticMeshRenderer ������Ʈ�� 1��1 �����ϴ� ����������Ʈ.
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
		
		//Constant Buffer�� ����.

	public:
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void First_BindBuffers() override;
		virtual void First_Render(const float* const dt) override;
		virtual void First_UnbindBuffers() override;
		
		Pg::Data::StaticMeshRenderer* GetStaticMeshRenderer() { return _staticMeshRenderer; }
	private:
		//�Ŀ� ���̴� �����ÿ� ���� ���� Vertex / Index Buffer ���ε�.
		void BindMainVertexIndexBuffer();


	private:
		//���� SkinnedMeshRenderer ĳ�����ؼ� �����ϰ� �ִ�.
		Pg::Data::StaticMeshRenderer* _staticMeshRenderer{ nullptr };
	
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectBase>> _cbFirst;
	};
}



