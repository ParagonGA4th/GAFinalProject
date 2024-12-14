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
		//virtual void Start() override;


	public:
		void SetSize(float width, float height);
		void SetWidth(float width);
		void SetHeight(float height);

		PGFLOAT2 GetSize();
		float GetWidth();
		float GetHeight();

		float _width;
		float _height;

		//0-100 사이의 범위 안에 있어야 한다!
		//0이 가장 앞, 100이 가장 뒤.
		unsigned int _sortingLayer = 1;
	};
}


