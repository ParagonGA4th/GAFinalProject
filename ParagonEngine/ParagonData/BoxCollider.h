#pragma once


/// <summary>
/// 변지상의 BoxCollider
/// Box형 충돌 디버그를 띄우기 위한 정보만 컴포넌트로 가지고 있을 예정
/// 2023.10.22
/// </summary>

namespace Pg::Data
{
	class GameObject;
}

namespace Pg::Engine
{
	class BoxCollider
	{
	public:
		BoxCollider(Pg::Data::GameObject* owner);

		void Update();

		float GetWidth() const;
		float GetHeight() const;
		float GetDepth() const;

	private:
		float _width;
		float _height;
		float _depth;
	};
}

