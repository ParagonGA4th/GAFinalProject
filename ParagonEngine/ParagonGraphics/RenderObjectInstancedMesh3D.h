#pragma once
#include "RenderObject3D.h"

/// <summary>
/// InstancedStaticMesh.
/// </summary>

namespace Pg::Graphics
{
	class RenderObjectInstancedMesh3D : public RenderObject3D
	{
	public:
		RenderObjectInstancedMesh3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID);
		~RenderObjectInstancedMesh3D();
	
		virtual void CreateObjMatBuffers() override {}

	public:
		//SkinnedMesh�� ���������� Ȱ���. �������� �����Ű�� ���ؼ�, �ִϸ��̼� ������ �� �ѹ�, �̸� �Ǵ��Ѵ�.
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void First_BindBuffers() override;
		virtual void First_Render(const float* const dt) override;
		virtual void First_UnbindBuffers() override;

	private:
		//�� RenderObject�� ������� ��ü�� ������ �ʴ´�.
		

	};
}

