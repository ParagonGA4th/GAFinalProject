#pragma once
#include "BaseRenderer.h"

/// <summary>
/// 모든 3D로 렌더할 렌더러 컴포넌트들의 부모.
/// </summary>
/// 
namespace Pg::Data
{
	class RendererBase3D : public BaseRenderer
	{
	public:
		RendererBase3D(GameObject* obj);
		virtual ~RendererBase3D();

		//구체화되지 않은 상태에서 해당 함수를 호출하면 에러.
		virtual void Render() override;

	private:
		
	};
}

