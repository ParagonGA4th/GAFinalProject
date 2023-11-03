#pragma once
#include "RenderObjectBase.h"
#include "../ParagonData/LightType.h"

#include "ConstantBufferDefine.h"
#include "ConstantBuffer.h"

#include "LightStructs.h"

namespace Pg::Graphics
{
	class LowDX11Storage;
	class VertexShader;
	class PixelShader;
	class ConstantBufferDefine;
	struct ConstantBufferDefine::cbPerObjectBase;


}

namespace Pg::Data
{
	class Light;
}



namespace Pg::Graphics
{
	class RenderObjectLight : public RenderObjectBase
	{
	public:
		RenderObjectLight();
		virtual ~RenderObjectLight();

	public:
		Pg::Data::Enums::eLightType _type;

	public:
		void Build();

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
		ConstantBufferDefine::cbPerObjectBase* _defaultConstantBufferStruct;

	private:
		LowDX11Storage* _DXStorage;

	};
}

