#pragma once

namespace Pg::Data
{
	class CameraData;
}

namespace Pg::Graphics
{
	class LowDX11Storage;
	class LowDX11Logic;

	class ForwardRenderer
	{
	public:
		ForwardRenderer();

	public:
		void Initialize();
		void BeginRender();
		void Render(Pg::Data::CameraData camData);
	
	private:
		LowDX11Storage* _DXStorage;
		LowDX11Logic* _DXLogic;
	};
}

