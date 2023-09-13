#pragma once

/// <summary>
/// 변지상의 엔진 인터페이스. 코어에 들어갈 예정
/// 2023.09.11
/// </summary>
namespace Pg::Core
{
	class IGraphics abstract
	{
	public:
		virtual void  Initialize() abstract;

		virtual void Update() abstract;

		virtual void Finalize() abstract;
	};
}