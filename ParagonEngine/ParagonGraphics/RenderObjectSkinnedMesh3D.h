#pragma once
#include "RenderObject3D.h"
#include "AssetAnimationDataDefine.h"
#include "../ParagonData/EditorMode.h"
#include <dxtk/SimpleMath.h>
#include <vector>
#include <memory>
#include <chrono>

/// <summary>
/// SkinnedMeshRenderer ������Ʈ�� 1��1 �����ϴ� ����������Ʈ.
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

		//Object-Material �����Ͱ� ���� ��Ī/�ε� �� ��, �ϰ������� �ߵ��� �Լ��̴�.	
		virtual void CreateObjMatBuffers() {};
	public:
		//SkinnedMesh�� ���������� Ȱ���. �������� �����Ű�� ���ؼ�, �ִϸ��̼� ������ �� �ѹ�, �̸� �Ǵ��Ѵ�.
		void UpdateAnimationInfo(const float* const dt, const Pg::Data::Enums::eEditorMode* const editorMode);
		virtual void First_UpdateConstantBuffers(Pg::Data::CameraData* camData) override;
		virtual void First_BindBuffers() override;
		virtual void First_Render(const float* const dt) override;
		virtual void First_UnbindBuffers() override;

		//���������� SetAnimation ȣ��. (Client �� ���� �ƴ� Internal Mechanic)
		void SetAnimation(const std::string& animName, bool isLoop);

		Pg::Data::AnimTransform* FindAnimTransform(const std::string& animNodeName);
		Pg::Data::SkinnedMeshRenderer* GetSkinnedMeshRenderer() { return _skinnedMeshRenderer; }

	private:
		//Start-End �ִϸ��̼� �ð� �Է�. Refresh
		//void RefreshStartEndAnim();

		//������ ����� ������Ʈ�ϱ�.
		void UpdateAnimMatrices(float dt);

		//�Ŀ� ���̴� �����ÿ� ���� ���� Vertex / Index Buffer ���ε�.
		void BindMainVertexIndexBuffer();

		//ConstantBufferUpdate.
		void UpdateMainCB(Pg::Data::CameraData* camData);
		void UpdateSkinnedCB();


	private:
		//���� SkinnedMeshRenderer ĳ�����ؼ� �����ϰ� �ִ�.
		Pg::Data::SkinnedMeshRenderer* _skinnedMeshRenderer{ nullptr };

	private:
		//���� ��� ���� �ִϸ��̼�.
		RenderAnimation* _currentAnim{ nullptr };

		//Animation Blending�� ����, ���� �ִ� Animation ����.
		//Blending ������, Nullptr�� ��ȯ.
		RenderAnimation* _prevAnim{ nullptr };

		//�ִϸ��̼� ��� ����. 
		//double _currentTick;
		//std::chrono::time_point<std::chrono::steady_clock> _startedTime;
		//std::chrono::time_point<std::chrono::steady_clock> _expectedEndTime;
		bool _isLoop{false};
		double _animationTime{ 0.0 }; // ���� �ִϸ��̼��� ����� �ð�
		double _currentTick{ 0.0 };	  // ���� ����ǰ� �ִ� ƽ.
		bool _isEndFrameCalled{false};
		float _blendLerpRatio{ 0.0f };
		
		float* ANIMATION_BLEND_SPEED_POINTER{ nullptr };
		int* ANIMATION_PAUSE_PROPERTY_POINTER{ nullptr };
	private:
		//���� Skinning ��ü�� �������� �ִϸ��̼��� �����ϱ� ���� �����ϴ� CopyNode.
		std::unique_ptr<ModifiedNode_SkinnedMesh> _copiedModifyRootNode;
		std::unordered_map<std::string, const ModifiedNode_SkinnedMesh*> _animatedModifNodeMap;

	private:
		//�迭��� ������ ��ĵ��� �����ϴ� ��.
		void FillInNodeBuffer(const ModifiedNode_SkinnedMesh* const selfNode);
		void FillInBoneBuffer(const ModifiedNode_SkinnedMesh* const selfNode);

	private:
		DirectX::SimpleMath::Vector3 FillPositionForNodeAnim(RenderAnimation* renderAnim, NodeAnim_AssetData* nodeAnim);
		DirectX::SimpleMath::Quaternion FillRotationForNodeAnim(RenderAnimation* renderAnim, NodeAnim_AssetData* nodeAnim);
		DirectX::SimpleMath::Vector3 FillScaleForNodeAnim(RenderAnimation* renderAnim, NodeAnim_AssetData* nodeAnim);


	private:
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectBase>> _cbFirstBase;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectSkinnedNodes>> _cbAllSkinnedNodes;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjectSkinnedBones>> _cbAllSkinnedBones;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbPerObjMatBase>> _cbObjMatBase;
	};
}

#pragma region OldSkinningHeaderFunc
//Skinned Mesh�� ��������.
		//void BoneTransformUpdate();

		//void ReadNodeHierarchy(double animTick, const aiNode* pNode, const aiAnimation* pAnim, DirectX::SimpleMath::Matrix parentTransform);

		//void CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& outQuat, double animTick, const NodeAnim_AssetData const* pNodeAnim);
		//void CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& outVec, double animTick, const NodeAnim_AssetData const* pNodeAnim);
		// 
		//unsigned int FindRotationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim);
		//unsigned int FindTranslationIndex(double animTick, const NodeAnim_AssetData const* pNodeAnim);
		//
		//Node�� ������Ʈ��Ų��.
		//void UpdateSingleNodeWithAnim(const Node_AssetData* const selfNode, const Node_AssetData* const parentNode, const NodeAnim_AssetData* const nodeAnim);

#pragma endregion OldSkinningHeaderFunc
