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

	class UtilMain : public Pg::Core::IUtil
	{
	public: 
		PARAGON_UTIL_DLL virtual void Initialize(float screenWidth, float screenHeight) override;
		PARAGON_UTIL_DLL virtual void Update() override;
		PARAGON_UTIL_DLL virtual void Finalize() override;

	private:
		Input::InputSystem* _inputSystem;
	};
}

