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

		void SetMeshFilePath(const std::string& meshFilePath);
		std::string GetMeshFilePath();

	private:
		std::string _meshFilePath;
	};
}

