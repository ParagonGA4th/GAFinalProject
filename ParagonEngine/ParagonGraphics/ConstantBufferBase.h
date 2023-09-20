#pragma once

namespace Pg::Graphics
{
	class ConstantBufferBase
	{
	public:
		ConstantBufferBase();
		virtual void Update() abstract;
	};
}


