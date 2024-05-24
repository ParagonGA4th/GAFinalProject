#pragma once
#include "SceneInformationList.h"
#include "../ParagonData/CameraData.h"
#include <array>

/// <summary>
/// 모든 SendReceiver들의 부모.
/// Send = CPU->GPU. // Receive = GPU -> CPU
/// 둘 중 하나만 수행하거나 / 둘 다 수행하거나 할 수 있다.
/// SceneInformationList 받아서 쓴다.
/// </summary>

namespace Pg::Graphics
{
	using SCArray = std::array<void*, 5>;
	class D3DCarrier;

	class ISendReceiver
	{
	public:
		virtual void Initialize() abstract;

		//만약 보내야 할 자료가 있으면 보내기.
		virtual void SendData(D3DCarrier* carrier, const SceneInformationList& info, const Pg::Data::CameraData* const camData) abstract;
		virtual void ProcessData() abstract;
		virtual void ReceiveData(SceneInformationList& info) abstract;
	};
}

