#pragma once
#include "RenderObject2D.h"
#include "../ParagonData/eFillRatioDirection.h"
#include <vector>
/// <summary>
/// ImageRenderer ������Ʈ�� 1��1 �����ϴ� ����������Ʈ.
/// </summary>

namespace Pg::Graphics
{
	class RenderTexture2D;
}

namespace Pg::Graphics
{
	class RenderObjectImage2D : public RenderObject2D
	{
	public:
		RenderObjectImage2D(Pg::Data::BaseRenderer* baseRenderer);

		virtual void Render(DirectX::SpriteBatch* spriteBatch, Pg::Data::CameraData* camData) override;
	
	
		//�ϴ��� ���Ƿ� FILL RATIO 100.0f �� ����.
		float* _fillRatio = nullptr;

		float* _imageWidth = nullptr;
		float* _imageHeight = nullptr;

		Pg::Data::eFillRatioDirection* _fillRatioDirection = nullptr;

		unsigned int GetImageIndex();
		void SetImageIndex(unsigned int val);

	private:
		//FillRatio�� ���� �ٸ� RECT�� ��ȯ��.
		void ReturnAppropriateFillRectOriginTrans(
			Pg::Data::eFillRatioDirection dir, RECT& outRect, DirectX::XMFLOAT2& outOrigin, DirectX::XMFLOAT2& outTrans);

	private:
		//RenderTexture2D* _texture2D;

		//���� �ε����� �ٲ� �� �ְ� �� ���̴�.
		std::vector<RenderTexture2D*> _texture2DVector;
		unsigned int _imageIndex{ 0 };
	};
}


