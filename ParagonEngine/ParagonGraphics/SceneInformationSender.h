#pragma once
#include "ISendReceiver.h"

namespace Pg::Graphics
{
	class SceneInformationSender : public ISendReceiver
	{
	public:
		SceneInformationSender();
		~SceneInformationSender();

		virtual void Initialize() override;

		virtual void SendData(const SceneInformationList& info, const Pg::Data::CameraData* const camData) override;
		virtual void ProcessData() override;
		virtual void ReceiveData(SceneInformationList& info) override;

	};
}


