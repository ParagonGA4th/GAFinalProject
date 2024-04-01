#pragma once
#include "RenderObject3D.h"
#include <dxtk/SimpleMath.h>
#include <vector>
#include <chrono>

/// <summary>
/// SkinnedMeshRenderer 컴포넌트와 1대1 대응하는 렌더오브젝트.
/// </summary>

namespace Pg::Data
{
	class SkinnedMeshRenderer;
}

namespace Pg::Graphics
{
	class RenderAnimation;
	class NodeAnim_AssetData;
	class Node_AssetData;
}

namespace Pg::Graphics
{
	class RenderObjectSkinnedMesh3D : public RenderObject3D
	{
		friend class ParagonRenderer;

	public:
		RenderObjectSkinnedMesh3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID);
		~RenderObjectSkinnedMesh3D();

		//Object-Material 데이터가 전부 매칭/로드 된 후, 일괄적으로 발동될 함수이다.	
		virtual void CreateObjMatBuffers() override;
	public:
		//SkinnedMesh에 예외적으로 활용됨. 프레임을 진행시키기 위해서, 애니메이션 로직을 딱 한번, 미리 판단한다.
		void UpdateAnimationInfo(const float* const dt);
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void First_BindBuffers() override;
		virtual void First_Render(const float* const dt) override;
		virtual void First_UnbindBuffers() override;

		virtual void ObjMat_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void ObjMat_BindBuffers() override;
		virtual void ObjMat_Render(const float* const dt) override;
		virtual void ObjMat_UnbindBuffers() override;

		//내부적으로 SetAnimation 호출. (Client 딴 제어 아닌 Internal Mechanic)
		void SetAnimation(const std::string& animName, bool isLoop);


	private:
		//Start-End 애니메이션 시간 입력. Refresh
		void RefreshStartEndAnim();

		//실제로 행렬을 업데이트하기.
		void UpdateAnimMatrices(float dt);

		//후에 셰이더 라이팅에 쓰일 메인 Vertex / Index Buffer 바인딩.
		void BindMainVertexIndexBuffer();

		//ConstantBufferUpdate.
		void UpdateMainCB(Pg::Data::CameraData* camData);
		void UpdateSkinnedCB();
		void UpdateObjMatBaseCB(Pg::Data::CameraData* camData);
		void UpdateObjMatSkinnedCB();

		void BindObjMatVertexIndexBuffer();
	private:
		//현재 재생 중인 애니메이션.
		RenderAnimation* _currentAnim;
		bool _isLoop{false};

		//애니메이션 재생 관리. 
		double _currentTick;
		std::chrono::time_point<std::chrono::steady_clock> _startedTime;
		std::chrono::time_point<std::chrono::steady_clock> _expectedEndTime;

		//Skinned Mesh에 독립적임.
		//void BoneTransformUpdate();

		//void ReadNodeHierarchy(double animTick, const aiNode* pNode, const aiAnimation* pAnim, DirectX::SimpleMath::Matrix parentTransform);
	private:
		void CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& outQuat, double animTick, const NodeAnim_AssetData const* pNodeAnim);
		void CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& outVec, double animTick, const NodeAnim_AssetData const* pNodeAnim);
		 
		unsigned int FindRotationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim);
		unsigned int FindTranslationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim);
		
		//Node를 업데이트시킨다.
		void UpdateSingleNodeWithAnim(const Node_AssetData* const selfNode, const Node_AssetData* const parentNode, const NodeAnim_AssetData* const nodeAnim);
	private:
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectBase>> _cbFirstBase;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectSkinnedNodes>> _cbAllSkinnedNodes;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectSkinnedBones>> _cbAllSkinnedBones;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjMatBase>> _cbObjMatBase;



	private:
		//GPU에 바인딩될 Bone Tranform Vector. 크기는 100이 기본.
		//FinalTransform 그자체.
		//얘는 이제 개별적인 RenderObject에 소속될 것이다!
		std::vector<DirectX::SimpleMath::Matrix> _boneTransformVector;

	};
}