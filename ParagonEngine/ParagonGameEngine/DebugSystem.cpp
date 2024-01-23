#include "DebugSystem.h"
#include "../ParagonAPI/KeyCodeType.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/SphereCollider.h"
#include "../ParagonData/PlaneCollider.h"
#include "../ParagonData/RayCast.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <cassert>

namespace Pg::Engine
{

	DebugSystem::DebugSystem() :
		_isDebug(true)
	{

	}

	void DebugSystem::Initialize()
	{
		// Input
		auto& tInputSystem = singleton<Input::InputSystem>();
		tInput = &tInputSystem;
	}


	void DebugSystem::Update(Pg::Data::Scene* scene)
	{
		using namespace Pg::Engine::Input;
		using namespace Pg::API::Input;

		//디버그 껐다 켜기.
		if (tInput->GetKeyDown(DebugOnOff))
		{
			SetDebugMode(!_isDebug);
		}

		//디버그 모드가 아닐 때 리턴.
	/*	if (!_isDebug)
		{
			return;
		}*/

		///Event System 들어오면 지울 것.
		//디버그 활성화 시 렌더링.
		if (_isDebug)
		{
			for (auto& it : scene->GetObjectList())
			{
				Pg::Data::BoxCollider* tBoxCol = it->GetComponent<Pg::Data::BoxCollider>();
				Pg::Data::CapsuleCollider* tCapsuleCol = it->GetComponent<Pg::Data::CapsuleCollider>();
				Pg::Data::SphereCollider* tShpereCol = it->GetComponent<Pg::Data::SphereCollider>();
				Pg::Data::PlaneCollider* tPlaneCol = it->GetComponent<Pg::Data::PlaneCollider>();
				Pg::Data::StaticBoxCollider* tStaticBoxCol = it->GetComponent<Pg::Data::StaticBoxCollider>();
				Pg::Data::RayCast* tRayCast = it->GetComponent<Pg::Data::RayCast>();

				if (tBoxCol != nullptr)
				{
					DrawBoxDebug(&(tBoxCol->_boxInfo));
				}
				else if (tStaticBoxCol != nullptr)
				{
					DrawBoxDebug(&(tStaticBoxCol->_boxInfo));
				}
				else if (tCapsuleCol != nullptr)
				{
					DrawCapsuleDebug(&(tCapsuleCol->_capsuleInfo));
				}
				else if (tShpereCol != nullptr)
				{
					DrawSphereDebug(&(tShpereCol->_sphereInfo));
				}
				else if (tPlaneCol != nullptr)
				{
					DrawPlaneDebug(&(tPlaneCol->_planeInfo));
				}
				else if (tRayCast != nullptr)
				{
					DrawRayCastDebug(&(tRayCast->_rayCastInfo));
				}
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

	void DebugSystem::DrawPlaneDebug(Pg::Data::PlaneInfo* planeInfo)
	{
		_planeVec.push_back(planeInfo);
	}

	void DebugSystem::DrawRayCastDebug(Pg::Data::RayCastInfo* rayCastInfo)
	{
		_rayCastVec.push_back(rayCastInfo);
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

	const std::vector<Pg::Data::PlaneInfo*>& DebugSystem::GetPlaneVector() const
	{
		return _planeVec;
	}

	const std::vector<Pg::Data::RayCastInfo*>& DebugSystem::GetRayCastVector() const
	{
		return _rayCastVec;
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

	void DebugSystem::DeletePlaneDebug()
	{
		_planeVec.clear();
	}

	void DebugSystem::DeleteRayCastDebug()
	{
		_rayCastVec.clear();
	}
}