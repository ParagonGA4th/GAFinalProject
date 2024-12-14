#pragma once
#include "SceneInformationList.h"
#include "../ParagonData/CameraData.h"
#include <array>

/// <summary>
/// ��� SendReceiver���� �θ�.
/// Send = CPU->GPU. // Receive = GPU -> CPU
/// �� �� �ϳ��� �����ϰų� / �� �� �����ϰų� �� �� �ִ�.
/// SceneInformationList �޾Ƽ� ����.
/// </summary>

namespace Pg::Graphics
{
	using SCArray = std::array<void*, 5>;
	class D3DCarrier;

	class ISendReceiver
	{
	public:
		virtual void Initialize() abstract;

		//���� ������ �� �ڷᰡ ������ ������.
		virtual void SendData(D3DCarrier* carrier, const SceneInformationList& info, const Pg::Data::CameraData* const camData) abstract;
		virtual void ProcessData() abstract;
		virtual void ReceiveData(SceneInformationList& info) abstract;
	};
}

