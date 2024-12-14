#pragma once
#include "../ParagonUtil/InputSystem.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonData/DebugData.h"
#include "../ParagonData/Scene.h"
#include <vector>
#include <string>

/// <summary>
/// �������� DebugSystem.
/// �⺻������ Collider�� Debug�� ��� �����̴�.
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
		//����� ������ ���Ϳ� ����.
		void DrawBoxDebug(Pg::Data::BoxInfo* boxInfo);
		void DrawBox2DDebug(Pg::Data::Box2DInfo box2DInfo);
		void DrawCapsuleDebug(Pg::Data::CapsuleInfo* capsuleInfo);
		void DrawSphereDebug(Pg::Data::SphereInfo* sphereInfo);
		void DrawPlaneDebug(Pg::Data::PlaneInfo* planeInfo);
		void DrawRayCastDebug(Pg::Data::RayCastInfo rayCastInfo);
		void DrawLineDebug(PGFLOAT3 begin, PGFLOAT3 end, PGFLOAT4 col);
		void DrawNavMeshDebug(Pg::Data::NavMeshInfo* navMeshInfo); // ��� �ٸ� �ֵ��� �� Delete�� �� ���� Delete�Ǵ� ���� �ƴ϶�, Scene�� ����� ������ ������ ������ ��. 
		void DrawNavCylinderDebug(Pg::Data::NavCylinderInfo* navCylinderInfo); // ��� ���� Delete �����س��� ����. ���� ���⿡ ���� ������ ��.

		void DeleteBoxDebug();
		void DeleteBox2DDebug();
		void DeleteSphereDebug();
		void DeleteCapsuleDebug();
		void DeletePlaneDebug();
		void DeleteLineDebug();
		void DeleteRayCastDebug();
		//void DeleteNavMeshDebug();  // ��� �ٸ� �ֵ��� �� Delete�� �� ���� Delete�Ǵ� ���� �ƴ϶�, Scene�� ����� ������ ������ ������ ��. 
		//void DeleteNavCylinderDebug(); //�ʿ��� �� ����� �� ��.
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
		const std::vector<Pg::Data::NavCylinderInfo*>& GetNavCylinderVector() const;

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
		std::vector<Pg::Data::NavCylinderInfo*> _navCylinderVec;

		//std::vector<Pg::Data::BoxInfo*> _boxVecChosen;
		//std::vector<Pg::Data::LineInfo*> _lineVecChosen;

		//����� ��� �÷���
		bool _isDebug;

		Pg::Util::Input::InputSystem* tInput = nullptr;
	};

}

