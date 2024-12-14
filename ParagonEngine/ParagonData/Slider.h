#pragma once
#include "Component.h"
#include "data_factory.h" // Auto Register�� ���� �ʼ����.

#include <functional>
/// <summary>
/// �������� Slider ������Ʈ.
/// Slider�� ���氪�� ���� �ݹ�Ǵ� �̺�Ʈ�� �־�� �Ѵ�.
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
	class Handle;

	class Slider : public Component, Pg::Factory::Data::RegisteredInFactory<Component, Slider, GameObject*>
	{
	public:
		Slider(GameObject* owner);

		static Component* CreateInstance(GameObject* go) { return new Slider(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::Slider"; }

		virtual void Internal_EngineAwake() override;
		virtual void Start() override;
		virtual void Update() override;

		void SetValueEvent(std::function<void(float)> event);
		std::function<void(float)> GetvalueEvent();

		void SetImagePath(const std::string path);

		void SetImageSize(float 
			, float height);
		void SetImageWidth(float width);
		void SetImageHeight(float height);

		float GetImageWidth();
		float GetImageHeight();

		void SetValue(float val);
		float GetValue();

	public:
		//Serialize�� ���� public����.
		Handle* handleObj;

	private:
		//�����̴��� ���� ����� �� �߻��ϴ� �̺�Ʈ.
		std::function<void(float)> _onValueEvent;
		float _value;

		ImageRenderer* _imageRenderer = nullptr;
		Pg::Util::Input::InputSystem* _inputSystem;

		

	public:
		//�̹����� ������
		float* _imageWidth;
		float* _imageHeight;

		//�����̴��� �ִ� �ּҰ�
		float _minWidth = 0.f;
		float _maxWidth = 1.f;
		float _minHeight = 0.f;
		float _maxHeight = 1.f;

		bool _isClick;

		void FindRequiredObject();
	};
}
