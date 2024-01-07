#pragma once
#include "RenderObject3D.h"

/// <summary>
/// SkinnedMeshRenderer 컴포넌트와 1대1 대응하는 렌더오브젝트.
/// </summary>

namespace Pg::Data
{
	class SkinnedMeshRenderer;
}

namespace Pg::Graphics
{
	class RenderObjectSkinnedMesh3D : public RenderObject3D
	{
	public:
		RenderObjectSkinnedMesh3D(Pg::Data::BaseRenderer* baseRenderer);
		virtual ~RenderObjectSkinnedMesh3D();

		//Object-Material 데이터가 전부 매칭/로드 된 후, 일괄적으로 발동될 함수이다.	
		virtual void CreateObjMatBuffers() override;
	public:
		virtual void First_Render() override;
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void First_BindBuffers() override;
		virtual void First_UnbindBuffers() override;

		virtual void ObjMat_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void ObjMat_BindBuffers() override;
		virtual void ObjMat_Render() override;
		virtual void ObjMat_UnbindBuffers() override;

	private:
		//Skinned Mesh에 독립적임.
		//void BoneTransformUpdate();

		//void ReadNodeHierarchy(double animTick, const aiNode* pNode, const aiAnimation* pAnim, DirectX::SimpleMath::Matrix parentTransform);
		//void CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& outQuat, double animTick, const aiNodeAnim* pNodeAnim);
		//void CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& outVec, double animTick, const aiNodeAnim* pNodeAnim);
		//
		//unsigned int FindRotation(double animTick, const aiNodeAnim* pNodeAnim);
		//unsigned int FindTranslation(double animTick, const aiNodeAnim* pNodeAnim);

	private:
		RenderTexture2D* _normal = nullptr;
		RenderTexture2D* _diffuse = nullptr;
	};
}