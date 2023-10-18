#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Pg::Editor::Helper
{
	class EditorHelper
	{
	public:
		void SetDevice(ID3D11Device* device);
		ID3D11Device* GetDevice();		
		
		void SetDeviceContext(ID3D11DeviceContext* deviceContext);
		ID3D11DeviceContext* GetDeviceContext();

	private:
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;
	};
}
