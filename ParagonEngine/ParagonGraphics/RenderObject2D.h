#pragma once
#include "GraphicsDLLExporter.h"
#include "RenderObjectBase.h"

#include <dxtk/SpriteBatch.h>
/// <summary>
/// GameObject�� �׷��ȿ����� ������ �����̶� 1��1 ������ �� �ְ� �ϴ� Ŭ�����̴�.
/// 2D ��ο� ȣȯ�� ���̴�.
/// 
/// Core���� BaseRenderer�� ���� GameObject�� :
/// RenderObject�� �׷��ȿ����� ���������� �������� �ʴ´�.
/// </summary>

namespace Pg::Graphics
{
	class RenderObject2D : public Pg::Graphics::RenderObjectBase
	{
	public:
		RenderObject2D(Pg::Data::BaseRenderer* baseRenderer);
		virtual ~RenderObject2D();

		virtual void Render(DirectX::SpriteBatch* spriteBatch, Pg::Data::CameraData* camData) abstract;

	protected:
		unsigned int* _sortingLayer = nullptr;
	};
}


