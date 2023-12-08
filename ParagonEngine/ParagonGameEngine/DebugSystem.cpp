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
		//디버그 모드가 아닐 때 리턴.
		if (!_isDebug)
		{
			return;
		}

		//Event System 들어오면 지울 것.
		for (auto& it : scene->GetObjectList())
		{
			Pg::Data::BoxCollider* tBoxCol = it->GetComponent<Pg::Data::BoxCollider>();
			Pg::Data::CapsuleCollider* tCapsuleCol = it->GetComponent<Pg::Data::CapsuleCollider>();
			Pg::Data::SphereCollider* tShpereCol = it->GetComponent<Pg::Data::SphereCollider>();
			
			if (tBoxCol != nullptr)
			{
				DrawBoxDebug(&(tBoxCol->_boxInfo));
			}
			else if (tCapsuleCol != nullptr)
			{
				DrawCapsuleDebug(&(tCapsuleCol->_capsuleInfo));
			}
			else if (tShpereCol != nullptr)
			{
				DrawSphereDebug(&(tShpereCol->_sphereInfo));
			}
		}

		DrawLineDebug({ 0.f,0.f,0.f }, {10.f,8.f,0.f}, { 1.f,1.f,0.f,1.f });
		DrawLineDebug({ 0.f,0.f,0.f }, {8.f,10.f,0.f}, { 1.f,1.f,0.f,1.f });

		assert(true);
	}	


	void DebugSystem::Finalize()
	{

	}

	void DebugSystem::SetDebugMode(bool isdebug)
	{
		_isDebug = isdebug;
	}

	void DebugSystem::DrawBoxDebug(Pg::Data::BoxInfo * boxInfo)
	{
		_boxVec.push_back(boxInfo);
	}


	void DebugSystem::DrawCapsuleDebug(Pg::Data::CapsuleInfo* capsuleInfo)
	{
		_capsuleVec.push_back(capsuleInfo);
	}


	void DebugSystem::DrawSphereDebug(Pg::Data::SphereInfo* sphereInfo)
	{
		_sphereVec.push_back(sphereInfo);
	}

	void DebugSystem::DrawLineDebug(PGFLOAT3 begin, PGFLOAT3 end, PGFLOAT4 col)
	{
		//[TW] Line 한정, 포인터가 되면 안된다.
		// 매번 변하는 위치 및 특성, 정해져 있으면 안된다! (특정 콜라이더에도 종속X이기에, 현 구조와 병행 불가)

		//Line Vector
		Pg::Data::LineInfo tLineInfo;
		tLineInfo.beginPoint = begin;
		tLineInfo.endPoint = end;
		tLineInfo.color = col;
		_lineVec.push_back(tLineInfo);
	}

	const std::vector<Pg::Data::BoxInfo*>& DebugSystem::GetBoxVector() const
	{
		return _boxVec;
	}

	const std::vector<Pg::Data::LineInfo>& DebugSystem::GetLineVector() const
	{
		return _lineVec;                                
	}

	const std::vector<Pg::Data::SphereInfo*>& DebugSystem::GetSphereVector() const
	{
		return _sphereVec;
	}

	const std::vector<Pg::Data::CapsuleInfo*>& DebugSystem::GetCapsuleVector() const
	{
		return _capsuleVec;
	}

	void DebugSystem::DeleteBoxDebug()
	{
		_boxVec.clear();
	}

	void DebugSystem::DeleteSphereDebug()
	{
		_sphereVec.clear();
	}

	void DebugSystem::DeleteCapsuleDebug()
	{
		_capsuleVec.clear();
	}

	void DebugSystem::DeleteLineDebug()
	{
		_lineVec.clear();
	}

	

}