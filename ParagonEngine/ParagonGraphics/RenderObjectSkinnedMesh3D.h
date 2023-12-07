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

		virtual void Render() override;

	private:
		virtual void BindBuffers() override;

	public:
		virtual void UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void BindConstantBuffers() override;
		virtual void UnbindConstantBuffers() override;

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