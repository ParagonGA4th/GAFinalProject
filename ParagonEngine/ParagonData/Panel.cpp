#include "Panel.h"

namespace Pg::Data
{
	Panel::Panel(GameObject* obj)
		: RendererBase2D(obj),
		_imagePath("")
	{

	}

	void Panel::SetImagePath(std::string path)
	{
		_imagePath = path;
	}

	std::string Panel::GetImagePath()
	{
		return _imagePath;
	}

}

