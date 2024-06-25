#include "ImageRenderer.h"
#include "../ParagonUtil/Log.h"

namespace Pg::Data
{
	ImageRenderer::ImageRenderer(GameObject* obj)
		: RendererBase2D(obj),
		_imagePath("")
	{
		SetRendererTypeName(typeid(this).name());
	}

	void ImageRenderer::Internal_EngineAwake()
	{
		
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

	void ImageRenderer::SetImageIndex(unsigned int val)
	{
		if (_setImageIndexFunc)
		{
			_setImageIndexFunc(val);
		}
		else
		{
			PG_WARN("Function Not Yet Bound!");
		}
	}

	unsigned int ImageRenderer::GetImageIndex()
	{
		unsigned int tRet = 0;

		if (_getImageIndexFunc)
		{
			tRet = _getImageIndexFunc();
		}
		else
		{
			PG_WARN("Function Not Yet Bound!");
		}

		return tRet;
	}

	void ImageRenderer::SetFillRatio(float val)
	{
		_fillRatio = std::clamp<float>(val, 0.f, 100.f);
	}

	float ImageRenderer::GetFillRatio()
	{
		return _fillRatio;
	}

	void ImageRenderer::SetFillRatioDirection(eFillRatioDirection val)
	{
		_fillRatioDirection = val;
	}

	eFillRatioDirection ImageRenderer::GetFillRatioDirection()
	{
		return _fillRatioDirection;
	}

}

