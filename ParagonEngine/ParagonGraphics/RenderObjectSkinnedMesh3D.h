#pragma once
#include "RenderObject3D.h"
#include <dxtk/SimpleMath.h>
#include <vector>

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
		friend class ParagonRenderer;

	public:
		RenderObjectSkinnedMesh3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID, unsigned int matID);
		virtual ~RenderObjectSkinnedMesh3D();

		//Object-Material 데이터가 전부 매칭/로드 된 후, 일괄적으로 발동될 함수이다.	
		virtual void CreateObjMatBuffers() override;
	public:
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void First_BindBuffers() override;
		virtual void First_Render(const float* const dt) override;
		virtual void First_UnbindBuffers() override;

		virtual void ObjMat_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void ObjMat_BindBuffers() override;
		virtual void ObjMat_Render(const float* const dt) override;
		virtual void ObjMat_UnbindBuffers() override;

		//내부적으로 SetAnimation 호출. (Client 딴 제어 아닌 Internal Mechanic)
		void SetAnimation(const std::string& animName);


	private:
		unsigned int tick;




		//Skinned Mesh에 독립적임.
		//void BoneTransformUpdate();

		//void ReadNodeHierarchy(double animTick, const aiNode* pNode, const aiAnimation* pAnim, DirectX::SimpleMath::Matrix parentTransform);
		//void CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& outQuat, double animTick, const aiNodeAnim* pNodeAnim);
		//void CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& outVec, double animTick, const aiNodeAnim* pNodeAnim);
		//
		//unsigned int FindRotation(double animTick, const aiNodeAnim* pNodeAnim);
		//unsigned int FindTranslation(double animTick, const aiNodeAnim* pNodeAnim);

	private:

	private:
		//GPU에 바인딩될 Bone Tranform Vector. 크기는 100이 기본.
		//FinalTransform 그자체.
		//얘는 이제 개별적인 RenderObject에 소속될 것이다!
		std::vector<DirectX::SimpleMath::Matrix> _boneTransformVector;
	};
}