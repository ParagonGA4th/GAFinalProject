#include "ImageRenderer.h"

namespace Pg::Data
{
	ImageRenderer::ImageRenderer(GameObject* obj)
		: RendererBase2D(obj),
		_imagePath("")
	{
		SetRendererTypeName(typeid(this).name());
	}

	void ImageRenderer::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper<ImageRenderer>(this, sv);
	}

	void ImageRenderer::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper<ImageRenderer>(this, sv);
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

