#pragma once
#include "Component.h"
#include "data_factory.h" // Auto Register를 위한 필수요건.

#include <functional>
/// <summary>
/// 변지상의 Slider 컴포넌트.
/// Slider의 변경값에 따라 콜백되는 이벤트가 있어야 한다.
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
		//Serialize를 위해 public으로.
		Handle* handleObj;

	private:
		//슬라이더의 값이 변경될 때 발생하는 이벤트.
		std::function<void(float)> _onValueEvent;
		float _value;

		ImageRenderer* _imageRenderer = nullptr;
		Pg::Util::Input::InputSystem* _inputSystem;

		

	public:
		//이미지의 사이즈
		float* _imageWidth;
		float* _imageHeight;

		//슬라이더의 최대 최소값
		float _minWidth = 0.f;
		float _maxWidth = 1.f;
		float _minHeight = 0.f;
		float _maxHeight = 1.f;

		bool _isClick;

		void FindRequiredObject();
	};
}
