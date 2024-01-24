#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"

#include <memory>
#include <string>

/// <summary>
/// 앞으로 렌더할 때 모든 기본 렌더러들이 상속받을 기본 클래스.
/// 모든 세부 구현 클래스들은 SetRendererTypeName을 typeid(this).name()으로 전달해야 한다!
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class Transform;
}

namespace Pg::Data
{
	class BaseRenderer : public Component
	{
	public:
		BaseRenderer(GameObject* obj);
		virtual ~BaseRenderer();

		//렌더링 로직 판단을 위해 실제 렌더러 타입 이름 반환.
		const std::string& GetRendererTypeName() const;

	protected:
		//Renderer의 실제 타입을 결정한다.
		void SetRendererTypeName(const std::string& typeName);

	private:
		// 어떤 Renderer가 실제로 만들어졌는지 연동하기 위해서.
		// 나중에는 어떤 렌더러가 추가되냐에 따라서 비트플래그가 설정되어야 할것.
		std::string _rendererTypeName; 
	};
}



