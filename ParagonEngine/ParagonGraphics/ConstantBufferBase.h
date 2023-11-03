#pragma once

/// <summary>
/// 런타임 다형성을 활용하기 위한 ConstantBuffer의 부모 클래스
/// 
/// 2023. 09. 21. 김정식
/// </summary>

struct ID3D11Buffer;

namespace Pg::Graphics
{
	class ConstantBufferBase
	{
	public:
		ConstantBufferBase();
		virtual ~ConstantBufferBase();

		virtual void UpdateAndBind(unsigned int num) abstract;
		virtual ID3D11Buffer* GetBuffer() abstract;

	};
}


