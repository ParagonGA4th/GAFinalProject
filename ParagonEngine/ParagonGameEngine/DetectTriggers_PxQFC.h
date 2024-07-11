#pragma once
#include <physx/PxQueryFiltering.h>
#include <physx/PxShape.h>

#include "../ParagonData/LayerMask.h"

//PxRaycast가 Trigger 오브젝트들을 통과할지, 아닐지 등을 필터링할 수 있게.

namespace Pg::Engine
{
	class DetectTriggers_PxQFC : public physx::PxQueryFilterCallback
	{
	public:
		DetectTriggers_PxQFC(bool detectTriggers, Pg::Data::Enums::eLayerMask excludeLayer) : _isDetectTriggers(detectTriggers), _excludeLayer(excludeLayer) {}

		physx::PxQueryHitType::Enum preFilter(const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags) override
		{
			//PxScene->raycast는 eNONE일 경우, false 리턴 // eTOUCH / eBLOCK일 경우, true 리턴.
			// PxShape가 없으면 Crash 막기 위해 Early Return.
			if (!shape)
			{
				return physx::PxQueryHitType::eNONE;
			}
				
			//제외할 레이어와 같으면 NONE 리턴.
			//const physx::PxFilterData shapeFilter = shape->getQueryFilterData();
			if (filterData.word0 == _excludeLayer)
			{
				return physx::PxQueryHitType::eNONE;
			}

			// Trigger라면, 저장된 값에 의해 조건문 분기.
			if (shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)
			{
				return _isDetectTriggers ? physx::PxQueryHitType::eTOUCH : physx::PxQueryHitType::eNONE;
			}

			//아니라면, Block되었다고 리턴.
			return physx::PxQueryHitType::eBLOCK;
		}

		physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit, const physx::PxShape* shape, const physx::PxRigidActor* actor) override
		{
			// 쓰이지 않음.
			return physx::PxQueryHitType::eBLOCK;
		}

	private:
		bool _isDetectTriggers;
		unsigned int _excludeLayer;
	};
}
