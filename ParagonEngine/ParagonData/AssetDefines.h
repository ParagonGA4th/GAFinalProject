#pragma once

/// Template Interface 패턴이 현재로서는 불가능하고, 상호참조 없이는 프로젝트 사이 연결이 불가능했기에, 
/// 인터페이스를 Enum 아래에 감추기 위해 enum class 정의해놓음.
/// 한 서브 리소스 클래스당 하나의 Enum이 해당된다.
/// 실제 캐스팅에서는 type_info.name()이 사용된다. 에디터를 위한 비주얼라이제이션 준비 겸.

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
		_3DMODEL, //Graphic Engine에서 Static / Skinned를 구분할 것.
		_FONT,
		_RENDER_VERTEXSHADER,
		_RENDER_PIXELSHADER,
		_RENDERMATERIAL,
		_ANIMATION
	};
}
