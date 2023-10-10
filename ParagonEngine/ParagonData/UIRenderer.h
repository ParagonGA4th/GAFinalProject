#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// 23.10.10 오수안
/// UI 렌더링을 위한 베이스 클래스. 모든 UI 컴포넌트는 해당 클래스를 상속한다
/// </summary>

namespace Pg::Data
{
	class GameObject;

	using namespace Pg::Math;

	class UIRenderer : public Component
	{
	public:
		UIRenderer(GameObject* obj);

	protected:
		virtual void Start() override;

	public:
		void SetSize(float width, float height);
		void SetWidth(float width);
		void SetHeight(float height);
		void SetIsFocused(bool isFocused);
		void SetIsClicked(bool isClicked);

		PGFLOAT2 GetSize();
		float GetWidth();
		float GetHeight();
		bool IsFocused();
		bool IsClicked();

	protected:
		float _width;
		float _height;
		bool _isFocused;
		bool _isClicked;
	};

}

