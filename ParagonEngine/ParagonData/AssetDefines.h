#pragma once

/// Template Interface ������ ����μ��� �Ұ����ϰ�, ��ȣ���� ���̴� ������Ʈ ���� ������ �Ұ����߱⿡, 
/// �������̽��� Enum �Ʒ��� ���߱� ���� enum class �����س���.
/// �� ���� ���ҽ� Ŭ������ �ϳ��� Enum�� �ش�ȴ�.
/// ���� ĳ���ÿ����� type_info.name()�� ���ȴ�. �����͸� ���� ���־�������̼� �غ� ��.

namespace Pg::Data::Enums
{
	enum class eAssetDefine
	{
		_NONE,
		_TEXTURE1D,
		_TEXTURE2D,
		_TEXTURE2DARRAY,
		_TEXTURECUBE,
		_CUBEMAP,
		_3DMODEL, //Graphic Engine���� Static / Skinned�� ������ ��.
		_FONT,
		_RENDER_VERTEXSHADER,
		_RENDER_PIXELSHADER,
		_RENDERMATERIAL,
		_ANIMATION
	};
}
