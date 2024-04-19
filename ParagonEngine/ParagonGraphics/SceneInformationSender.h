#pragma once
#include "ISendReceiver.h"
#include "ConstantBuffer.h"
#include "ConstantBufferDefine.h"

namespace Pg::Graphics
{
	class LowDX11Storage;
}

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

	private:
		void CreateBuffers();
		void ProcessSceneInfoData();
		void ProcessLightInfoData();

	private:
		


	private:
		LowDX11Storage* _DXStorage = nullptr;
		//잠시 CameraData 구조체를 보관해놓는다.
		const Pg::Data::CameraData* _savedCamData = nullptr;
		const SceneInformationList* _savedSceneInfo = nullptr;

	private:
		//SceneInfo Constant Buffer.
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbSceneInfo>> _cbSceneInfo;
		std::unique_ptr<ConstantBuffer<ConstantBufferDefine::cbRenderingInfo>> _cbRenderingInfo;

	};
}


