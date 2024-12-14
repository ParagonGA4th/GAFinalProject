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

		void SetRendererOffset(Pg::Math::PGFLOAT3 vecOffset) { _rendererOffset = vecOffset; }
		Pg::Math::PGFLOAT3 GetRendererOffset() { return _rendererOffset; }

		void SetRendererRotationOriginOffset(Pg::Math::PGFLOAT3 vecOffset);
		Pg::Math::PGFLOAT3 GetRendererRotationOriginOffset() { return _rotationOriginOffset; }

		bool GetIsUseRotationOriginOffset() { return _isUseRotationOriginOffset; }
	protected:
		std::string _meshFilePath;
		std::string _renderMaterialPath;	

		bool _isInstanced{ false };
		bool _isAlphaClipped{ false };

		//개별적인 Offset. XML에 기록되지 않고,
		//Component들이 쉽게 세팅할 수 있게 기록.
		//렌더러 Offset 설정.
		Pg::Math::PGFLOAT3 _rendererOffset;

		//얘는 Rotation Origin까지 영향을 미칠 것이다.
		Pg::Math::PGFLOAT3 _rotationOriginOffset;

		//안 쓰면 무조건 False. Set을 해야 반영이 될 것이다.
		bool _isUseRotationOriginOffset{ false };
	};
}

