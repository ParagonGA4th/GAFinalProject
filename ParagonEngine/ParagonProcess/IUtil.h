#pragma once
#include <string>

/// <summary>
/// ParagonUtil (UtilMain)�� �����ϱ� ���� �������̽�.
/// </summary>

namespace Pg::Core
{
	class IUtil abstract
	{
	public:
		virtual void Initialize(const std::string& resourceListPath, float screenWidth, float screenHeight, void* hwnd) abstract;
		virtual void Update() abstract;
		virtual void Finalize() abstract;
	};
}
