#pragma once

/// <summary>
/// 변지상의 엔진 인터페이스. 코어에 들어갈 예정
/// 2023.09.11
/// </summary>
namespace Pg::Core
{
	class IEngine abstract
	{
	public:
		virtual void Initialize(float width, float height) abstract;

		virtual void Update() abstract;

		virtual void Finalize() abstract;

		//그래픽스 엔진에서 필요한 정보
		//virtual Scene* GetCurrentScene();
	};
}