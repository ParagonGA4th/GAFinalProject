#pragma once
#include "BaseRenderer.h"

/// <summary>
/// ��� 3D�� ������ ������ ������Ʈ���� �θ�.
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
		//���ο��� ��� Path���� �ش� �Լ� ȣ�� �������� ä������ ���̴�, Instanced üũ ���� �Ǵܱ���.
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

		//�������� Offset. XML�� ��ϵ��� �ʰ�,
		//Component���� ���� ������ �� �ְ� ���.
		//������ Offset ����.
		Pg::Math::PGFLOAT3 _rendererOffset;

		//��� Rotation Origin���� ������ ��ĥ ���̴�.
		Pg::Math::PGFLOAT3 _rotationOriginOffset;

		//�� ���� ������ False. Set�� �ؾ� �ݿ��� �� ���̴�.
		bool _isUseRotationOriginOffset{ false };
	};
}

