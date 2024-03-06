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

		virtual void ConvertPotentialUnrealValues() abstract;
		virtual void CheckForPathNameErrors() abstract;

		void SetMeshFilePath(const std::string& meshFilePath);
		std::string GetMeshFilePath();

		void SetMaterialFilePath(const std::string& materialFilePath);
		std::string GetMaterialFilePath();

	protected:
		std::string _meshFilePath;
		std::string _renderMaterialPath;
	};
}

