#include "RenderObject3DList.h"
#include <functional>
#include <numeric>
#include <dxtk/SimpleMath.h>
#include <DirectXMath.h>
#include "MathHelper.h"
#include "Asset3DModelData.h"
#include "AssetModelDataDefine.h"

namespace Pg::Graphics
{


	std::vector<RenderObject3D*> RenderObject3DList::GetRenderObjectWithGameObject(Pg::Data::GameObject* obj)
	{
		std::vector<RenderObject3D*> tRet;

		//Instanced는 변할 이유가 없음, 지원하지 않음.

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
		//없다면 점검할 필요 조차 없다.
		//Instanced는 지워질 일 없음, 제외됨.

		//하나하나 vector.
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

		//일단 카메라와 상대적인 거리를 기준으로 판단 
		for (auto& it : _allAlphaBlendedList)
		{
			it->_cameraRelativeDistSquared = Pg::Math::PGFloat3LengthSquared(camData->_position - it->_obj->_transform._position);
		}

		//해당 로직이 작동하기 위한 기본적인 전제. 같은 사이즈 : 대응되는 인덱스가 활용되어야 한다.
		if (_allAlphaBlendedList.size() != _sortedIndexBlendedVec.size())
		{
			_sortedIndexBlendedVec.resize(_allAlphaBlendedList.size());
		}

		//기본 인덱스 설정하기 위해, iota 사용, 하나씩 증가.
		std::iota(_sortedIndexBlendedVec.begin(), _sortedIndexBlendedVec.end(), 0);
		std::sort(_sortedIndexBlendedVec.begin(), _sortedIndexBlendedVec.end(), [&](int i, int j) {return _allAlphaBlendedList[i] > _allAlphaBlendedList[j]; });

		//내림차순으로 원본 데이터의 인덱스를 옮기지 않고 어느 순서대로 렌더할지를 별도로 _sortedIndexBlendedVec에 보관했다.
		// Alpha Blending의 기본적인 Back-To-Front 조건을 만든다.
		// std::less의 경우.
		// 원본 = 5 7 8
		//	 b = 1 3 2
		//이런 식으로 볼 수 있게 되는 것. (우리는 반대)
	}

	void RenderObject3DList::UpdateObjectCullingState(Pg::Data::CameraData* camData)
	{
		DirectX::XMMATRIX tViewMat = Helper::MathHelper::PG2XM_MATRIX(camData->_viewMatrix);
		DirectX::XMMATRIX tProjMat = Helper::MathHelper::PG2XM_MATRIX(camData->_projMatrix);
			
		DirectX::BoundingFrustum tFrustum;
		DirectX::BoundingFrustum::CreateFromMatrix(tFrustum, DirectX::XMMatrixMultiply(tViewMat, tProjMat));

		//DirectX::SimpleMath::

		for (auto& [bRenderMat, bVecPtr] : _staticList)
		{
			for (int i = 0; i < bVecPtr->size(); i++)
			{
				//개별적인 RenderObject 단위.
				bool tShouldBeCulled = true;
				RenderObjectStaticMesh3D* tROMesh = bVecPtr->at(i).second.get();
				for (auto& tActualModelMesh : tROMesh->_modelData->_assetSceneData->_meshList)
				{
					if (tFrustum.Intersects(tActualModelMesh._AABB))
					{
						// 하나라도 Intersect하면, 렌더해줘야 한다.
						tShouldBeCulled = false;
						break;
					}
				}
				tROMesh->SetIsCulledFromRendering(tShouldBeCulled);
			}
		}

		for (auto& [bRenderMat, bVecPtr] : _skinnedList)
		{
			for (int i = 0; i < bVecPtr->size(); i++)
			{
				//개별적인 RenderObject 단위.
				bool tShouldBeCulled = true;
				RenderObjectSkinnedMesh3D* tROMesh = bVecPtr->at(i).second.get();
				for (auto& tActualModelMesh : tROMesh->_modelData->_assetSceneData->_meshList)
				{
					if (tFrustum.Intersects(tActualModelMesh._AABB))
					{
						// 하나라도 Intersect하면, 렌더해줘야 한다.
						tShouldBeCulled = false;
						break;
					}
				}
				tROMesh->SetIsCulledFromRendering(tShouldBeCulled);
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
					if (tFrustum.Intersects(tActualModelMesh._AABB))
					{
						// 하나라도 Intersect하면, 렌더해줘야 한다.
						tShouldBeCulled = false;
						break;
					}
				}
				tROMesh->SetIsCulledFromRendering(tShouldBeCulled);
			}
			else
			{
				bool tShouldBeCulled = true;
				RenderObjectStaticMesh3D* tROMesh = it->_eitherStaticMesh.get();
				for (auto& tActualModelMesh : tROMesh->_modelData->_assetSceneData->_meshList)
				{
					if (tFrustum.Intersects(tActualModelMesh._AABB))
					{
						// 하나라도 Intersect하면, 렌더해줘야 한다.
						tShouldBeCulled = false;
						break;
					}
				}
				tROMesh->SetIsCulledFromRendering(tShouldBeCulled);
			}
		}
	}

}