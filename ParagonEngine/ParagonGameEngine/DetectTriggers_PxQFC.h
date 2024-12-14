#pragma once
#include <physx/PxQueryFiltering.h>
#include <physx/PxShape.h>

#include "../ParagonData/LayerMask.h"

//PxRaycast�� Trigger ������Ʈ���� �������, �ƴ��� ���� ���͸��� �� �ְ�.

namespace Pg::Engine
{
	class DetectTriggers_PxQFC : public physx::PxQueryFilterCallback
	{
	public:
		DetectTriggers_PxQFC(bool detectTriggers) : _isDetectTriggers(detectTriggers) {}

		physx::PxQueryHitType::Enum preFilter(const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags) override
		{
			//PxScene->raycast�� eNONE�� ���, false ���� // eTOUCH / eBLOCK�� ���, true ����.
			// PxShape�� ������ Crash ���� ���� Early Return.
			if (!shape)
			{
				return physx::PxQueryHitType::eNONE;
			}
				
			//������ ���̾�� ������ NONE ����.
			//const physx::PxFilterData shapeFilter = shape->getQueryFilterData();
			////bool tOne = (filterData.word0 == _excludeLayer);
			////bool tTwo = (shapeFilter.word0 == _excludeLayer);
			//if ((filterData.word0 == _excludeLayer) || (shapeFilter.word0 == _excludeLayer))
			//{
			//	return physx::PxQueryHitType::eNONE;
			//}

			// Trigger���, ����� ���� ���� ���ǹ� �б�.
			if (shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)
			{
				return _isDetectTriggers ? physx::PxQueryHitType::eTOUCH : physx::PxQueryHitType::eNONE;
			}

			//�ƴ϶��, Block�Ǿ��ٰ� ����.
			return physx::PxQueryHitType::eBLOCK;
		}

		physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit, const physx::PxShape* shape, const physx::PxRigidActor* actor) override
		{
			// ������ ����.
			return physx::PxQueryHitType::eBLOCK;
		}

	private:
		bool _isDetectTriggers;
	};
}
