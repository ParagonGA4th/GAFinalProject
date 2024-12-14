#include "RenderObject3DList.h"
#include <functional>
#include <numeric>
#include <dxtk/SimpleMath.h>
#include <DirectXMath.h>
#include "MathHelper.h"
#include "Asset3DModelData.h"
#include "AssetModelDataDefine.h"
#include "../ParagonUtil/Log.h"

namespace Pg::Graphics
{


	std::vector<RenderObject3D*> RenderObject3DList::GetRenderObjectWithGameObject(Pg::Data::GameObject* obj)
	{
		std::vector<RenderObject3D*> tRet;

		//Instanced�� ���� ������ ����, �������� ����.

		for (auto& it : this->_staticList)
		{
			for (auto& [go, ro] : *(it.second.get()))
			{
				if (go == obj)
				{
					tRet.push_back(ro.get());
				}
			}
		}

		for (auto& it : this->_skinnedList)
		{
			for (auto& [go, ro] : *(it.second.get()))
			{
				if (go == obj)
				{
					tRet.push_back(ro.get());
				}
			}
		}

		for (auto& it : this->_allAlphaBlendedList)
		{
			if (it->_obj == obj)
			{
				if (it->_isSkinned)
				{
					tRet.push_back(it->_eitherSkinnedMesh.get());
				}
				else
				{
					tRet.push_back(it->_eitherStaticMesh.get());
				}
			}
		}

		return tRet;
	}

	void RenderObject3DList::DeleteRenderObjectWithGameObject(Pg::Data::GameObject* obj)
	{
		//���ٸ� ������ �ʿ� ���� ����.
		//Instanced�� ������ �� ����, ���ܵ�.

		//�ϳ��ϳ� vector.
		if (!_staticList.empty())
		{
			for (auto& [bMat, bVec] : this->_staticList)
			{
				auto tVec = bVec.get();
				if (!tVec->empty())
				{
					tVec->erase(std::remove_if(tVec->begin(), tVec->end(), [&obj](auto& tPair) { return tPair.first == obj; }));
				}
			}
		}

		if (!_skinnedList.empty())
		{
			for (auto& [bMat, bVec] : this->_skinnedList)
			{
				auto tVec = bVec.get();
				if (!tVec->empty())
				{
					tVec->erase(std::remove_if(tVec->begin(), tVec->end(), [&obj](auto& tPair) { return tPair.first == obj; }));
				}
			}
		}

		if (!_allAlphaBlendedList.empty())
		{
			for (auto& it : this->_allAlphaBlendedList)
			{
				if (it->_obj == obj)
				{
					it->_eitherStaticMesh.reset();
					it->_eitherSkinnedMesh.reset();
				}
			}
		}
	}

	void RenderObject3DList::SortBlendedByDepth_BackToFront(Pg::Data::CameraData* camData)
	{
		if (_allAlphaBlendedList.empty())
		{
			return;
		}

		//�ϴ� ī�޶�� ������� �Ÿ��� �������� �Ǵ� 
		for (auto& it : _allAlphaBlendedList)
		{
			it->_cameraRelativeDistSquared = Pg::Math::PGFloat3LengthSquared(camData->_position - it->_obj->_transform._position);
		}

		//�ش� ������ �۵��ϱ� ���� �⺻���� ����. ���� ������ : �����Ǵ� �ε����� Ȱ��Ǿ�� �Ѵ�.
		if (_allAlphaBlendedList.size() != _sortedIndexBlendedVec.size())
		{
			_sortedIndexBlendedVec.resize(_allAlphaBlendedList.size());
		}

		//�⺻ �ε��� �����ϱ� ����, iota ���, �ϳ��� ����.
		std::iota(_sortedIndexBlendedVec.begin(), _sortedIndexBlendedVec.end(), 0);
		std::sort(_sortedIndexBlendedVec.begin(), _sortedIndexBlendedVec.end(), [&](int i, int j) {return _allAlphaBlendedList[i] > _allAlphaBlendedList[j]; });

		//������������ ���� �������� �ε����� �ű��� �ʰ� ��� ������� ���������� ������ _sortedIndexBlendedVec�� �����ߴ�.
		// Alpha Blending�� �⺻���� Back-To-Front ������ �����.
		// std::less�� ���.
		// ���� = 5 7 8
		//	 b = 1 3 2
		//�̷� ������ �� �� �ְ� �Ǵ� ��. (�츮�� �ݴ�)
	}

	void RenderObject3DList::UpdateObjectCullingState(Pg::Data::CameraData* camData)
	{
		using namespace DirectX;
		//�ϴ��� ���̰� ���� �ϱ� ���ؼ�, ����. ��ǥ ������ �̾ �Ұ�.
		//�� �� �� �� �� �� �� ��
		return;
		///�������� �𸣰�����.. => �Ųٷ� ����ȴ�. �̰� ���ľ�.

		DirectX::XMMATRIX tViewMat = Helper::MathHelper::PG2XM_MATRIX(camData->_viewMatrix);
		DirectX::XMMATRIX tProjMat = Helper::MathHelper::PG2XM_MATRIX(camData->_projMatrix);
		DirectX::XMMATRIX tVP = DirectX::XMMatrixMultiply(tViewMat, tProjMat);
		DirectX::BoundingFrustum tFrustum;
		DirectX::BoundingFrustum::CreateFromMatrix(tFrustum, tVP);

		//�ش� AABB���� Transform�� ���� �̵�������� �Ѵ�.

		//���� ������.
		int tRenderCount = 0;

		for (auto& [bRenderMat, bVecPtr] : _staticList)
		{
			for (int i = 0; i < bVecPtr->size(); i++)
			{
				//�������� RenderObject ����.
				bool tShouldBeCulled = true;
				RenderObjectStaticMesh3D* tROMesh = bVecPtr->at(i).second.get();
				for (auto& tActualModelMesh : tROMesh->_modelData->_assetSceneData->_meshList)
				{
					DirectX::BoundingOrientedBox tOrientedBoundary;
					tOrientedBoundary.Center = tActualModelMesh._AABB.Center;
					tOrientedBoundary.Extents = tActualModelMesh._AABB.Extents; //Quaternion�� ����Ʈ ����.
					Pg::Data::Transform* tObjTrans = &(tROMesh->GetBaseRenderer()->_object->_transform);
					DirectX::XMMATRIX tWorldTM = Pg::Math::PG2XM_MATRIX4X4(tObjTrans->GetWorldTM());
					DirectX::XMMATRIX tTransViewMat = Pg::Math::PG2XM_MATRIX4X4(
						Pg::Math::GetViewMatrixFromTransformValues(tObjTrans->GetRight(), tObjTrans->GetUp(), tObjTrans->GetForward(), tObjTrans->_position));
					tOrientedBoundary.Transform(tOrientedBoundary, DirectX::XMMatrixInverse(nullptr, tTransViewMat) * tWorldTM);

					if (tFrustum.Intersects(tOrientedBoundary))
					{
						// �ϳ��� Intersect�ϸ�, ��������� �Ѵ�.
						tShouldBeCulled = false;
						tRenderCount++;
						break;
					}
				}
				tROMesh->SetIsCulledFromRendering(tShouldBeCulled);
				//tROMesh->SetIsCulledFromRendering(!tShouldBeCulled);
			}
		}

		for (auto& [bRenderMat, bVecPtr] : _skinnedList)
		{
			for (int i = 0; i < bVecPtr->size(); i++)
			{
				//�������� RenderObject ����.
				bool tShouldBeCulled = true;
				RenderObjectSkinnedMesh3D* tROMesh = bVecPtr->at(i).second.get();
				for (auto& tActualModelMesh : tROMesh->_modelData->_assetSceneData->_meshList)
				{
					DirectX::BoundingOrientedBox tOrientedBoundary;
					tOrientedBoundary.Center = tActualModelMesh._AABB.Center;
					tOrientedBoundary.Extents = tActualModelMesh._AABB.Extents; //Quaternion�� ����Ʈ ����.
					Pg::Data::Transform* tObjTrans = &(tROMesh->GetBaseRenderer()->_object->_transform);
					DirectX::XMMATRIX tWorldTM = Pg::Math::PG2XM_MATRIX4X4(tObjTrans->GetWorldTM());
					DirectX::XMMATRIX tTransViewMat = Pg::Math::PG2XM_MATRIX4X4(
						Pg::Math::GetViewMatrixFromTransformValues(tObjTrans->GetRight(), tObjTrans->GetUp(), tObjTrans->GetForward(), tObjTrans->_position));
					tOrientedBoundary.Transform(tOrientedBoundary, DirectX::XMMatrixInverse(nullptr, tTransViewMat) * tWorldTM);

					if (tFrustum.Intersects(tOrientedBoundary))
					{
						// �ϳ��� Intersect�ϸ�, ��������� �Ѵ�.
						tShouldBeCulled = false;
						tRenderCount++;
						break;
					}
				}
				tROMesh->SetIsCulledFromRendering(tShouldBeCulled);
				//tROMesh->SetIsCulledFromRendering(!tShouldBeCulled);
			}
		}

		for (auto& it : _allAlphaBlendedList)
		{
			if (it->_isSkinned)
			{
				bool tShouldBeCulled = true;
				RenderObjectSkinnedMesh3D* tROMesh = it->_eitherSkinnedMesh.get();
				for (auto& tActualModelMesh : tROMesh->_modelData->_assetSceneData->_meshList)
				{
					DirectX::BoundingOrientedBox tOrientedBoundary;
					tOrientedBoundary.Center = tActualModelMesh._AABB.Center;
					tOrientedBoundary.Extents = tActualModelMesh._AABB.Extents; //Quaternion�� ����Ʈ ����.
					Pg::Data::Transform* tObjTrans = &(tROMesh->GetBaseRenderer()->_object->_transform);
					DirectX::XMMATRIX tWorldTM = Pg::Math::PG2XM_MATRIX4X4(tObjTrans->GetWorldTM());
					DirectX::XMMATRIX tTransViewMat = Pg::Math::PG2XM_MATRIX4X4(
						Pg::Math::GetViewMatrixFromTransformValues(tObjTrans->GetRight(), tObjTrans->GetUp(), tObjTrans->GetForward(), tObjTrans->_position));
					tOrientedBoundary.Transform(tOrientedBoundary, DirectX::XMMatrixInverse(nullptr, tTransViewMat) * tWorldTM);

					if (tFrustum.Intersects(tOrientedBoundary))
					{
						// �ϳ��� Intersect�ϸ�, ��������� �Ѵ�.
						tShouldBeCulled = false;
						tRenderCount++;
						break;
					}
				}
				tROMesh->SetIsCulledFromRendering(tShouldBeCulled);
				//tROMesh->SetIsCulledFromRendering(!tShouldBeCulled);
			}
			else
			{
				bool tShouldBeCulled = true;
				RenderObjectStaticMesh3D* tROMesh = it->_eitherStaticMesh.get();
				for (auto& tActualModelMesh : tROMesh->_modelData->_assetSceneData->_meshList)
				{
					DirectX::BoundingOrientedBox tOrientedBoundary;
					tOrientedBoundary.Center = tActualModelMesh._AABB.Center;
					tOrientedBoundary.Extents = tActualModelMesh._AABB.Extents; //Quaternion�� ����Ʈ ����.
					Pg::Data::Transform* tObjTrans = &(tROMesh->GetBaseRenderer()->_object->_transform);
					DirectX::XMMATRIX tWorldTM = Pg::Math::PG2XM_MATRIX4X4(tObjTrans->GetWorldTM());
					DirectX::XMMATRIX tTransViewMat = Pg::Math::PG2XM_MATRIX4X4(
						Pg::Math::GetViewMatrixFromTransformValues(tObjTrans->GetRight(), tObjTrans->GetUp(), tObjTrans->GetForward(), tObjTrans->_position));
					tOrientedBoundary.Transform(tOrientedBoundary, DirectX::XMMatrixInverse(nullptr, tTransViewMat) * tWorldTM);

					if (tFrustum.Intersects(tOrientedBoundary))
					{
						// �ϳ��� Intersect�ϸ�, ��������� �Ѵ�.
						tShouldBeCulled = false;
						tRenderCount++;
						break;
					}
				}
				tROMesh->SetIsCulledFromRendering(tShouldBeCulled);
				//tROMesh->SetIsCulledFromRendering(!tShouldBeCulled);
			}
		}

		//PG_TRACE(std::to_string(tRenderCount).c_str());
	}

}