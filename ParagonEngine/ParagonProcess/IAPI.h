#pragma once

///APIMain�� �����ϱ� ���� �������̽�.

namespace Pg::Core
{
	class IAPI abstract
	{
	public:
		virtual void Initialize() abstract;
		virtual void UpdateLoop() abstract;
		virtual void Finalize() abstract;
	};
}
