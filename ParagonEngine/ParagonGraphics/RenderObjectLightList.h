#pragma once
#include "../ParagonMath/PgMath.h"
#include "ConstantBuffer.h"

#include "LightStructs.h"

namespace Pg::Graphics
{
	class RenderObjectLight;
}

namespace Pg::Graphics
{
	struct RenderObjectLightList
	{

	public:
		RenderObjectLightList();

	public:
		std::vector<RenderObjectLight*> _list;
		

	public:
		void UpdateConstantBuffer();

	public:
		// 상수 버퍼들을 저장하는 벡터
		std::vector< ConstantBufferBase* > _constantBuffers;

		// 상수 버퍼 데이터를 추가하는 함수
		template <typename T>
		void CreateConstantBuffer(T* cbData)
		{
			ConstantBufferBase* tCBuffer = new ConstantBuffer<T>(cbData);
			_constantBuffers.emplace_back(tCBuffer);
		}

	private:
		// 상수 버퍼에 대응하는 구조체 멤버
		Pg::Data::Structs::Lights* _LightConstantBufferStruct;

	private:
		LowDX11Storage* _DXStorage;
	};
}
