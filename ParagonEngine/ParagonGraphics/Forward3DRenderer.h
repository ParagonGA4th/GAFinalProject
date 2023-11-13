#pragma once

namespace Pg::Data
{
	class CameraData;
}

namespace Pg::Graphics
{
	class LowDX11Storage;
	class LowDX11Logic;

	class Forward3DRenderer
	{
	public:
		Forward3DRenderer();

	public:
		void Initialize();
		void BeginRender();
		void Render(Pg::Data::CameraData camData);
	
	private:
		LowDX11Storage* _DXStorage;
		LowDX11Logic* _DXLogic;
	};
}

