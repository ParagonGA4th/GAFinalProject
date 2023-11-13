#pragma once
/// 게임엔진에서 콜라이더 등 
/// 디버그 렌더 과정을

namespace Pg::Data
{
	class CameraData;
}

namespace Pg::Graphics
{
	class LowDX11Storage;
	class LowDX11Logic;

	class DebugRenderer
	{
	public:
		DebugRenderer();

	public:
		void Initialize();
		void BeginRender();
		void Render(Pg::Data::CameraData* camData);

	private:
		LowDX11Storage* _DXStorage;
		LowDX11Logic* _DXLogic;

	private:



	};
}

