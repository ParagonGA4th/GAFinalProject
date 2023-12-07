#include "DebugSystem.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/SphereCollider.h"
#include "../ParagonUtil/Log.h"

#include <cassert>

namespace Pg::Engine
{

	void DebugSystem::Initialize()
	{

	}


	void DebugSystem::Update(Pg::Data::Scene* scene)
	{
		//Event System 들어오면 지울 것.
		for (auto& it : scene->GetObjectList())
		{
			Pg::Data::BoxCollider* tBoxCol = it->GetComponent<Pg::Data::BoxCollider>();
			
			if (tBoxCol != nullptr)
			{
				DrawBoxDebug(&(tBoxCol->_boxInfo));
			}

			Pg::Data::CapsuleCollider* tCapsuleCol = it->GetComponent<Pg::Data::CapsuleCollider>();

			if (tCapsuleCol != nullptr)
			{
				DrawCapsuleDebug(&(tCapsuleCol->_capsuleInfo));
			}

			Pg::Data::SphereCollider* tShpereCol = it->GetComponent<Pg::Data::SphereCollider>();

			if (tCapsuleCol != nullptr)
			{
				DrawSphereDebug(&(tShpereCol->_sphereInfo));
			}
		}

		assert(true);
	}	


	void DebugSystem::Finalize()
	{

	}

	void DebugSystem::SetDebugMode(bool isdebug)
	{
		_isDebug = isdebug;
	}

	void DebugSystem::DrawBoxDebug(Pg::Data::BoxInfo* boxInfo)
	{
		if (!_isDebug)
		{
			return;
		}

		_boxVec.push_back(boxInfo);
	}


	void DebugSystem::DrawCapsuleDebug(Pg::Data::CapsuleInfo* capsuleInfo)
	{
		if (!_isDebug)
		{
			return;
		}

		_capsuleVec.push_back(capsuleInfo);
	}


	void DebugSystem::DrawSphereDebug(Pg::Data::SphereInfo* sphereInfo)
	{
		if (!_isDebug)
		{
			return;
		}

		_sphereVec.push_back(sphereInfo);
	}

	void DebugSystem::DrawLineDebug(PGFLOAT3 beginPoint, PGFLOAT3 endPoint, PGFLOAT4 color)
	{

	}

	void DebugSystem::DeleteBoxDebug()
	{
		_boxVec.clear();
	}

	const std::vector<Pg::Data::BoxInfo*>& DebugSystem::GetBoxVector() const
	{
		return _boxVec;
	}

	const std::vector<Pg::Data::LineInfo*>& DebugSystem::GetLineVector() const
	{
		return _lineVec;                                
	}

}