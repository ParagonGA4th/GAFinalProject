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
		//내부엑서 모든 Path값이 해당 함수 호출 시점에서 채워졌을 것이니, Instanced 체크 여부 판단까지.
		virtual void CheckForPathNameErrors() abstract;

		
		void SetMeshFilePath(const std::string& meshFilePath);
		std::string GetMeshFilePath();

		void SetMaterialFilePath(const std::string& materialFilePath);
		std::string GetMaterialFilePath();

		bool GetIsInstanced() { return _isInstanced; }
		bool GetIsAlphaClipped() { return _isAlphaClipped; }

	protected:
		std::string _meshFilePath;
		std::string _renderMaterialPath;

		bool _isInstanced{ false };
		bool _isAlphaClipped{ false };
	};
}

