#pragma once
#include "Component.h"
#include "data_factory.h" // Auto Register를 위한 필수요건.

/// <summary>
/// 슬라이더에 붙일 Handle 클래스.
/// 얘는 그냥 Transform이랑 ImageRenderer만 들고 있으면 됨.
/// 자식 객체로 넣을 예정.
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

