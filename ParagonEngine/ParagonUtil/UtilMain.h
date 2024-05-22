#pragma once
#include "../ParagonProcess/IUtil.h"
#include "UtilDLLExporter.h"

/// <summary>
/// ParagonUtil의 체계를 관리하는 UtilMain.
/// </summary>
namespace Pg::Util
{
	namespace Input
	{
		class InputSystem;
	}

	namespace Time
	{
		class TimeSystem;
	}
	class InstancingException;

	class UtilMain : public Pg::Core::IUtil
	{
	public: 
		PARAGON_UTIL_DLL virtual void Initialize(const std::string& resourceListPath, float screenWidth, float screenHeight, void* hwnd) override;
		PARAGON_UTIL_DLL virtual void Update() override;
		PARAGON_UTIL_DLL virtual void Finalize() override;

	private:
		Input::InputSystem* _inputSystem;
		Time::TimeSystem* _timeSystem;
		InstancingException* _instancingException;
	};
}

