#pragma once
#include "Component.h"
#include "data_factory.h" // Auto Register�� ���� �ʼ����.

/// <summary>
/// �����̴��� ���� Handle Ŭ����.
/// ��� �׳� Transform�̶� ImageRenderer�� ��� ������ ��.
/// �ڽ� ��ü�� ���� ����.
/// 2023.03.08
/// </summary>
namespace Pg::Data
{
	class GameObject;
	class ImageRenderer;

	class Handle : public Component, Pg::Factory::Data::RegisteredInFactory<Component, Handle, GameObject*>
	{
	public:
		Handle(GameObject* owner);

		static Component* CreateInstance(GameObject* go) { return new Handle(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::Handle"; }

		virtual void Internal_EngineAwake() override;

		void SetImagePath(const std::string& path);
		void SetImageSize(float width, float height);

		void SetImageWidth(float width);
		float GetImageWidth();

		void SetImageHeight(float height);
		float GetImageHeight();

		//virtual void Update() override;

	private:
		ImageRenderer* _imageRenderer = nullptr;

		float* _imageWidth;
		float* _imageHeight;
	};
}

