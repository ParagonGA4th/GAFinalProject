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
	
		virtual void CreateObjMatBuffers() override;

	public:
		//SkinnedMesh에 예외적으로 활용됨. 프레임을 진행시키기 위해서, 애니메이션 로직을 딱 한번, 미리 판단한다.
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void First_BindBuffers() override;
		virtual void First_Render(const float* const dt) override;
		virtual void First_UnbindBuffers() override;

		virtual void ObjMat_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void ObjMat_BindBuffers() override;
		virtual void ObjMat_Render(const float* const dt) override;
		virtual void ObjMat_UnbindBuffers() override;


	private:
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjMatBase>> _cbObjMat;

	};
}

