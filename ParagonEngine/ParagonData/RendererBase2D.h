#pragma once
#include "BaseRenderer.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// 모든 2D로 렌더할 렌더러 컴포넌트들의 부모.
/// </summary>

namespace Pg::Data
{
	using namespace Pg::Math;

	class RendererBase2D : public BaseRenderer
	{
	public:
		RendererBase2D(GameObject* obj);
		virtual ~RendererBase2D();

	protected:
		virtual void Start() override;

		//구체화되지 않은 상태에서 해당 함수를 호출하면 에러.
		virtual void Render() override;

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


