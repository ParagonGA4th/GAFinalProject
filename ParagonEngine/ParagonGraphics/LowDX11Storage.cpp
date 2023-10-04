#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	LowDX11Storage::LowDX11Storage()
		:_hWnd(NULL),
		_screenWidth(0), _screenHeight(0),
		_device(nullptr), _deviceContext(nullptr),
		_swapChainDesc(), _bufferDesc(), _depthStencilDesc(), _depthStencilViewDesc(), _shaderResourceViewDesc(),
		_factory(nullptr), _swapChain(nullptr),_backBuffer(nullptr), _mainRTV(nullptr),
		_depthStencilBuffer(), _depthStencilState(), _depthStencilView(), _depthStencilSRV(),
		_solidDesc(), _wireframeDesc(), _solidState(), _wireframeState()
	{

	}

	LowDX11Storage* LowDX11Storage::GetInstance()
	{
		static LowDX11Storage* tInstance = new LowDX11Storage();

		return tInstance;
	}
	

}