#pragma once
#include <string>

/// <summary>
/// ParagonUtil (UtilMain)을 제어하기 위한 인터페이스.
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
