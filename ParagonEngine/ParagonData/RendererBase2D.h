#pragma once
#include "BaseRenderer.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// ��� 2D�� ������ ������ ������Ʈ���� �θ�.
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


