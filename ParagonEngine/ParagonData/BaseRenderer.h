#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"

#include <memory>
#include <string>

/// <summary>
/// ������ ������ �� ��� �⺻ ���������� ��ӹ��� �⺻ Ŭ����.
/// ��� ���� ���� Ŭ�������� SetRendererTypeName�� typeid(this).name()���� �����ؾ� �Ѵ�!
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

		//������ ���� �Ǵ��� ���� ���� ������ Ÿ�� �̸� ��ȯ.
		const std::string& GetRendererTypeName() const;

	protected:
		//Renderer�� ���� Ÿ���� �����Ѵ�.
		void SetRendererTypeName(const std::string& typeName);

	private:
		// � Renderer�� ������ ����������� �����ϱ� ���ؼ�.
		// ���߿��� � �������� �߰��ǳĿ� ���� ��Ʈ�÷��װ� �����Ǿ�� �Ұ�.
		std::string _rendererTypeName; 
	};
}



