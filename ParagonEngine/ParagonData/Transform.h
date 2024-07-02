#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"
#include "data_factory.h" // Auto Register를 위한 필수요건.
#include <memory>

/// <summary>
/// 23.10.6 변지상 && 오수안
/// 변지상이 프레임을 짜고 오수안이 구현한 Trasnform 클래스
/// 오브젝트의 자식과 부모, transform 종속성 등은 모두 여기서 관리한다
/// 또한 오브젝트의 오일러, 쿼터니언 변환을 위한 함수가 존재.
/// 
/// 24.01.03 최민서
/// 기존 Get, Set 방식에서 DataSerializer에 쓰일 visit_struct 구조 방식으로 변경.
/// </summary>

namespace Pg::Editor::Window { class Hierarchy; }

namespace Pg::Data
{
	using namespace Pg::Math;

	class GameObject;

	class Transform : public Component, Pg::Factory::Data::RegisteredInFactory<Component, Transform, GameObject*>
	{
		friend class Camera;
		friend class Pg::Editor::Window::Hierarchy;
		friend class StaticMeshRenderer;
		friend class SkinnedMeshRenderer;
	public:
		// 임시 기본생성자
		Transform() = default;
		Transform(GameObject* obj);

		//자동화된 Auto-Registration 작동 위해 필수.
		static Component* CreateInstance(GameObject* go) { return new Transform(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::Transform"; }

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		//// Get 월드 함수들
		//PGFLOAT3 GetPosition() const;
		//PGQuaternion GetRotation() const;
		//PGFLOAT3 GetScale() const;

		//// Get 로컬 함수들
		//PGFLOAT3 GetLocalScale();
		//PGFLOAT3 GetLocalPosition();
		//PGQuaternion GetLocalRotation();

		//// Set 월드 함수들
		//void SetPosition(const PGFLOAT3& pos);
		//void SetPosition(float x, float y, float z);
		//void SetRotation(const PGQuaternion& rot);
		//void SetRotation(float w, float x, float y, float z);
		//void SetScale(const PGFLOAT3& scale);
		//void SetScale(float x, float y, float z);

		//// Set 로컬 함수들
		//void SetLocalPosition(float x, float y, float z);
		//void SetLocalPosition(PGFLOAT3& pos);

		//void SetLocalRotation(float w, float x, float y, float z);
		//void SetLocalRotation(PGQuaternion& rot);
		//void SetLocalScale(float x, float y, float z);
		//void SetLocalScale(PGFLOAT3& sca);
		//void SetLocalRotationEuler(float x, float y, float z);
		//void SetLocalRotationEuler(PGFLOAT3& euler);

		// 각 로컬, 월드 transform을 위한 행렬
		PGQuaternion NormalizeQuaternion(PGQuaternion q);

		PGFLOAT4X4 GetLocalTranslateMatrix();
		PGFLOAT4X4 GetLocalScaleMatrix();
		PGFLOAT4X4 GetLocalRotationMatrix();

		PGFLOAT4X4 GetWorldTranslateMatrix();
		PGFLOAT4X4 GetWorldScaleMatrix();
		PGFLOAT4X4 GetWorldRotationMatrix();

		// 오브젝트의 전방, 상단, 오른쪽 벡터 (기즈모를 그리거나... 할 때 활용)
		PGFLOAT3 GetForward();
		PGFLOAT3 GetUp();
		PGFLOAT3 GetRight();

		// 짐벌락 방지를 위한... 오일러 && 쿼터니언 변환 함수들
		PGQuaternion EulerToQuaternion(float x, float y, float z);
		PGQuaternion EulerToQuaternion(const PGFLOAT3& euler);
		PGFLOAT3 QuaternionToEuler(float w, float x, float y, float z);
		PGFLOAT3 QuaternionToEuler(const PGQuaternion& quaternion);

		// 월드 행렬 (부모 유무를 고려)
		PGFLOAT4X4 GetWorldTM();
		PGFLOAT3 GetWorldPosition();
		PGQuaternion GetWorldRotation();
		PGFLOAT4X4 GetLocalTM();

		// 종속성 세팅 함수
		Transform* GetParent();
		const std::vector<Transform*>& GetChildren();
		Transform* GetChild(int index);
		Transform* GetChild(std::string name);

		bool HasParent();
		void AddChild(Transform* child);
		void AddChild(GameObject* child);

		void RemoveChild(GameObject* child);
		void RemoveChild(std::string child);


		/// 오브젝트의 3D 여부를 세팅... Transform에 있는게 맞을까?
		void SetIs3D(bool is3D);
		bool Is3D();

		//Culling을 위해, Graphics가 사용하는 헬퍼 함수.
		//스케일의 x,y,z 중 홀수개가 음수인가?
		bool IsScaleOddMinus();

		//이동이 가능한지 값 반환. (인스턴싱 관련)
		bool GetCanMove();

	public:
		/// visit_struct 방식
		BEGIN_VISITABLES(Transform);
		VISITABLE(PGFLOAT3, _position);
		VISITABLE(PGQuaternion, _rotation);
		VISITABLE(PGFLOAT3, _scale);
		END_VISITABLES;

	private:
		// 카메라를 위한 transform 함수
		// 카메라에서 rotation이 일어나면 오브젝트의 방향 벡터를 변환해준다
		//PGFLOAT3 SetForwardUpRight(PGFLOAT3 up, PGFLOAT3 right, PGFLOAT3 forward);
		
		// 부모, 자식 객체를 가리키는 transform
		Transform* _parent{ nullptr };
		std::vector<Transform*> _children;

		// Forward, Right, Up
		PGFLOAT3 _forward;
		PGFLOAT3 _right;
		PGFLOAT3 _up;

		// bool 
		bool _is3D;

		//초기값 True.
		bool _isCanMove;
	};
}

