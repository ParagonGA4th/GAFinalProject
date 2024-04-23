#pragma once
#include "RenderObject3D.h"
#include "AssetAnimationDataDefine.h"
#include "../ParagonData/EditorMode.h"
#include <dxtk/SimpleMath.h>
#include <vector>
#include <memory>
#include <chrono>

/// <summary>
/// SkinnedMeshRenderer 컴포넌트와 1대1 대응하는 렌더오브젝트.
/// </summary>

namespace Pg::Data
{
	class SkinnedMeshRenderer;
	class AnimTransform;
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
		void UpdateAnimationInfo(const float* const dt, const Pg::Data::Enums::eEditorMode* const editorMode);
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

		Pg::Data::AnimTransform* FindAnimTransform(const std::string& animNodeName);

	private:
		//Start-End 애니메이션 시간 입력. Refresh
		//void RefreshStartEndAnim();

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

		//애니메이션 재생 관리. 
		//double _currentTick;
		//std::chrono::time_point<std::chrono::steady_clock> _startedTime;
		//std::chrono::time_point<std::chrono::steady_clock> _expectedEndTime;
		bool _isLoop{false};
		double _animationTime{ 0.0 }; // 현재 애니메이션이 재생된 시간
		double _currentTick{ 0.0 };	  // 현재 재생되고 있는 틱.

	private:
		//개별 Skinning 객체별 독립적인 애니메이션을 실행하기 위해 존재하는 CopyNode.
		std::unique_ptr<ModifiedNode_SkinnedMesh> _copiedModifyRootNode;
		std::unordered_map<std::string, const ModifiedNode_SkinnedMesh*> _animatedModifNodeMap;

	private:
		//배열대로 실제로 행렬들을 투입하는 것.
		void FillInNodeBuffer(const ModifiedNode_SkinnedMesh* const selfNode);
		void FillInBoneBuffer(const ModifiedNode_SkinnedMesh* const selfNode);
	
	private:
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectBase>> _cbFirstBase;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectSkinnedNodes>> _cbAllSkinnedNodes;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectSkinnedBones>> _cbAllSkinnedBones;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjMatBase>> _cbObjMatBase;
	};
}

#pragma region OldSkinningHeaderFunc
//Skinned Mesh에 독립적임.
		//void BoneTransformUpdate();

		//void ReadNodeHierarchy(double animTick, const aiNode* pNode, const aiAnimation* pAnim, DirectX::SimpleMath::Matrix parentTransform);

		//void CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& outQuat, double animTick, const NodeAnim_AssetData const* pNodeAnim);
		//void CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& outVec, double animTick, const NodeAnim_AssetData const* pNodeAnim);
		// 
		//unsigned int FindRotationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim);
		//unsigned int FindTranslationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim);
		//
		//Node를 업데이트시킨다.
		//void UpdateSingleNodeWithAnim(const Node_AssetData* const selfNode, const Node_AssetData* const parentNode, const NodeAnim_AssetData* const nodeAnim);

#pragma endregion OldSkinningHeaderFunc
