#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/DebugData.h"
#include "data_factory.h" // Auto Register�� ���� �ʼ����.

#include <functional>

/// <summary>
/// ��ư�� ���� Ŭ����. 
/// 23.10.10
/// </summary>
namespace Pg::Util
{
	namespace Input
	{
		class InputSystem;
	}
}

namespace Pg::Data
{
	class GameObject;
	class ImageRenderer;

	class Button : public Component, Pg::Factory::Data::RegisteredInFactory<Component, Button, GameObject*>
	{
	public:
		Button(GameObject* owner);

		static Component* CreateInstance(GameObject* go) { return new Button(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::Button"; }

	public:
		virtual void GrabManagedObjects() override;
		virtual void Update() override;

		void ClickDown();
		void Hover();
		void NotHover();
		void ClickUp();

		//Script ��� ���� API�� ����
		void SetOnClickDownEvent(std::function<void()> onClickEvent);
		std::function<void()> GetOnClickDownEvent() const;

		void SetOnClickUpEvent(std::function<void()> onClickEvent);
		std::function<void()> GetOnClickUpEvent() const;

		void SetHover(std::function<void()> hover);
		std::function<void()> GetHover() const;

		void SetNotHover(std::function<void()> notHover);
		std::function<void()> GetNotHover() const;

		void SetImagePath(const std::string& path);
		void SetImageSize(float width, float height);
		
		void SetImageWidth(float width);
		float GetImageWidth();
		
		void SetImageHeight(float height);
		float GetImageHeight();

		ImageRenderer* GetImageRenderer();

	public:
		//�̹����� ������
		float* _imageWidth;
		float* _imageHeight;

	public:
		//Debug System�� ���
		Box2DInfo _box2dInfo;

	private:
		ImageRenderer* _imageRenderer = nullptr;
		Pg::Util::Input::InputSystem* _inputSystem;

		std::function<void()> _onClickEvent;
		std::function<void()> _onClickUpEvent;
		std::function<void()> _hover;
		std::function<void()> _notHover;

		bool _isPressed;	//��ư ���� ���� üũ.
	};
}


