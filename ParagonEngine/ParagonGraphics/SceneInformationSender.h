#pragma once
#include "ISendReceiver.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"
#include <memory>

namespace Pg::Util
{
	namespace Time { class TimeSystem; }
}

namespace Pg::Graphics
{
	class LowDX11Storage;
	//class D3DCarrier;
}

namespace Pg::Graphics
{
	class SceneInformationSender : public ISendReceiver
	{
	public:
		SceneInformationSender();
		~SceneInformationSender();

		virtual void Initialize() override;

		virtual void SendData(D3DCarrier* carrier, const SceneInformationList& info, const Pg::Data::CameraData* const camData) override;
		virtual void ProcessData() override;
		virtual void ReceiveData(SceneInformationList& info) override;

	private:
		void CreateBuffers();
		void ProcessSceneInfoData();
		void ProcessLightInfoData();

	private:
		


	private:
		LowDX11Storage* _DXStorage = nullptr;
		//��� CameraData ����ü�� �����س��´�.
		const Pg::Data::CameraData* _savedCamData = nullptr;
		const SceneInformationList* _savedSceneInfo = nullptr;
		D3DCarrier* _carrier = nullptr;

	private:
		//SceneInfo Constant Buffer.
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbSceneInfo>> _cbSceneInfo;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbRenderingInfo>> _cbRenderingInfo;

	private:
		Pg::Util::Time::TimeSystem* _timeSystem{ nullptr };
		float _currentRecordDegrees{ 0.f };
	};
}


