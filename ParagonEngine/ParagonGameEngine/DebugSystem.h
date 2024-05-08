#pragma once
#include "../ParagonUtil/InputSystem.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonData/DebugData.h"
#include "../ParagonData/Scene.h"
#include <vector>
#include <string>

/// <summary>
/// 변지상의 DebugSystem.
/// 기본적으로 Collider의 Debug를 띄울 예정이다.
/// 2023.11.09
/// </summary>

namespace Pg::Data
{
	class Scene;
}

namespace Pg::Engine
{
	using namespace Pg::Math;

	class InputSystem;

	class DebugSystem
	{
	public:
		DebugSystem();

		void Initialize();

		void EnableToggleDebugOnOff();
		void Update(Pg::Data::Scene* scene);

		void Finalize();

	public:
		//디버그 정보를 벡터에 저장.
		void DrawBoxDebug(Pg::Data::BoxInfo* boxInfo);
		void DrawBox2DDebug(Pg::Data::Box2DInfo box2DInfo);
		void DrawCapsuleDebug(Pg::Data::CapsuleInfo* capsuleInfo);
		void DrawSphereDebug(Pg::Data::SphereInfo* sphereInfo);
		void DrawPlaneDebug(Pg::Data::PlaneInfo* planeInfo);
		void DrawRayCastDebug(Pg::Data::RayCastInfo rayCastInfo);
		void DrawLineDebug(PGFLOAT3 begin, PGFLOAT3 end, PGFLOAT4 col);
		void DrawNavMeshDebug(Pg::Data::NavMeshInfo* navMeshInfo); // 얘는 다른 애들이 다 Delete될 때 같이 Delete되는 것이 아니라, Scene이 변경될 때마다 별개로 동작할 것. 

		void DeleteBoxDebug();
		void DeleteBox2DDebug();
		void DeleteSphereDebug();
		void DeleteCapsuleDebug();
		void DeletePlaneDebug();
		void DeleteLineDebug();
		void DeleteRayCastDebug();
		//void DeleteNavMeshDebug();  // 얘는 다른 애들이 다 Delete될 때 같이 Delete되는 것이 아니라, Scene이 변경될 때마다 별개로 동작할 것. 

		void SetDebugMode(bool isdebug);
		bool GetDebugMode();

	public:
		const std::vector<Pg::Data::BoxInfo*>& GetBoxVector() const;
		const std::vector<Pg::Data::Box2DInfo>& GetBox2DVector() const;
		const std::vector<Pg::Data::LineInfo>& GetLineVector() const;
		const std::vector<Pg::Data::SphereInfo*>& GetSphereVector() const;
		const std::vector<Pg::Data::CapsuleInfo*>& GetCapsuleVector() const;
		const std::vector<Pg::Data::PlaneInfo*>& GetPlaneVector() const;
		const std::vector<Pg::Data::NavMeshInfo*>& GetNavMeshVector() const;
		const std::vector<Pg::Data::RayCastInfo>& GetRayCastVector() const;

		//std::vector<Pg::Data::BoxInfo*>& GetDrawEnabledBoxVector() const;
		//std::vector<Pg::Data::LineInfo*>& GetDrawEnabledLineVector();
	private:
		std::vector<Pg::Data::BoxInfo*> _boxVec;
		std::vector<Pg::Data::Box2DInfo> _box2dVec;
		std::vector<Pg::Data::LineInfo> _lineVec;
		std::vector<Pg::Data::RayCastInfo> _rayCastVec;
		std::vector<Pg::Data::CapsuleInfo*> _capsuleVec;
		std::vector<Pg::Data::SphereInfo*> _sphereVec;
		std::vector<Pg::Data::PlaneInfo*> _planeVec;
		std::vector<Pg::Data::NavMeshInfo*> _navMeshVec;

		//std::vector<Pg::Data::BoxInfo*> _boxVecChosen;
		//std::vector<Pg::Data::LineInfo*> _lineVecChosen;

		//디버그 모드 플래그
		bool _isDebug;

		Pg::Util::Input::InputSystem* tInput = nullptr;
	};

}

