#pragma once
#include "../ParagonMath/PgMath.h"
#include <behaviortree_cpp_v3/basic_types.h>
/// <summary>
/// Groot�� ���� ��� �ʱ� ��ȯ�� ����, Template Specialization�� ����ؾ� �Ѵ�.
/// XML �δ��� Ŀ���� �ڷ����� �ε��� �� �������� �Ϸ���, �̿� ���� ���� �ʿ�.
/// XML�δ����� �̿� ���� ���� ������, �ʿ� X.
/// https://www.behaviortree.dev/docs/tutorial-basics/tutorial_03_generic_ports
/// </summary>

namespace BT
{
	//String -> PGFLOAT2 ��ȯ ���������� ����.
	template <> inline Pg::Math::PGFLOAT2 convertFromString<Pg::Math::PGFLOAT2>(StringView str)
	{
		//���������� Semicolon�� ���� �������ٰ� ����.
		auto parts = splitString(str, ';');

		//PGFloat2�̴�. 2���� üũ.
		if (parts.size() != 2)
		{
			throw RuntimeError("Invalid Input For ParagonEngine Data");
		}
		else
		{
			Pg::Math::PGFLOAT2 output;
			output.x = convertFromString<double>(parts[0]);
			output.y = convertFromString<double>(parts[1]);
			return output;
		}
	}

	//String -> PGFLOAT3 ��ȯ ���������� ����.
	template <> inline Pg::Math::PGFLOAT3 convertFromString<Pg::Math::PGFLOAT3>(StringView str)
	{
		//���������� Semicolon�� ���� �������ٰ� ����.
		auto parts = splitString(str, ';');

		//PGFloat3�̴�. 3���� üũ.
		if (parts.size() != 3)
		{
			throw RuntimeError("Invalid Input For ParagonEngine Data");
		}
		else
		{
			Pg::Math::PGFLOAT3 output;
			output.x = convertFromString<double>(parts[0]);
			output.y = convertFromString<double>(parts[1]);
			output.z = convertFromString<double>(parts[2]);
			return output;
		}
	}

	//String -> PGFLOAT4 ��ȯ ���������� ����.
	template <> inline Pg::Math::PGFLOAT4 convertFromString<Pg::Math::PGFLOAT4>(StringView str)
	{
		//���������� Semicolon�� ���� �������ٰ� ����.
		auto parts = splitString(str, ';');

		//PGFloat4�̴�. 4���� üũ.
		if (parts.size() != 4)
		{
			throw RuntimeError("Invalid Input For ParagonEngine Data");
		}
		else
		{
			Pg::Math::PGFLOAT4 output;
			output.x = convertFromString<double>(parts[0]);
			output.y = convertFromString<double>(parts[1]);
			output.z = convertFromString<double>(parts[2]);
			output.w = convertFromString<double>(parts[3]);
			return output;
		}
	}

	//String -> PGQuaternion ��ȯ ���������� ����.
	template <> inline Pg::Math::PGQuaternion convertFromString<Pg::Math::PGQuaternion>(StringView str)
	{
		//���������� Semicolon�� ���� �������ٰ� ����.
		auto parts = splitString(str, ';');

		//PGQuaternion�̴�. 4���� üũ.
		if (parts.size() != 4)
		{
			throw RuntimeError("Invalid Input For ParagonEngine Data");
		}
		else
		{
			Pg::Math::PGQuaternion output;
			output.w = convertFromString<double>(parts[0]);
			output.x = convertFromString<double>(parts[1]);
			output.y = convertFromString<double>(parts[2]);
			output.z = convertFromString<double>(parts[3]);
			return output;
		}
	}
}