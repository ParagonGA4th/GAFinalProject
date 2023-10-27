#pragma once
#include "BaseRenderer.h"
#include <string>
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

		void SetMeshFilePath(const std::string& meshFilePath);
		std::string GetMeshFilePath();

	private:
		std::string _meshFilePath;
	};
}

