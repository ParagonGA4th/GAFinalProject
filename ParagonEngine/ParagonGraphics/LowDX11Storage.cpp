#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	LowDX11Storage::LowDX11Storage()
		:_hWnd(NULL),
		_device(nullptr), _deviceContext(nullptr),
		_factory(nullptr), _swapChainDesc(), _swapChain(nullptr),
		_backBuffer(nullptr), _mainRTV(nullptr)
	{

	}
}