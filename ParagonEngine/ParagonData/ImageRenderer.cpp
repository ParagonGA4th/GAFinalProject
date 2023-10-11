#include "ImageRenderer.h"

namespace Pg::Data
{
	ImageRenderer::ImageRenderer(GameObject* obj)
		: RendererBase2D(obj),
		_imagePath("")
	{

	}

	void ImageRenderer::SetImagePath(std::string path)
	{
		_imagePath = path;
	}

	std::string ImageRenderer::GetImagePath()
	{
		return _imagePath;
	}

}

