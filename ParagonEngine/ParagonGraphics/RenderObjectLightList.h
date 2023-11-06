#pragma once
#include "../ParagonMath/PgMath.h"
#include "ConstantBuffer.h"

#include "LightStructs.h"

namespace Pg::Graphics
{
	class RenderObjectLight;
}

namespace Pg::Data
{
	class Light;
}

namespace Pg::Graphics
{
	struct RenderObjectLightList
	{

	public:
		RenderObjectLightList();

	public:
		//std::vector<void*> _data;
		//std::vector<Pg::Data::Structs::Light*> _data;
		

	public:
		void UpdateConstantBuffer();
		void ParseLights(Pg::Data::Light* lightComponent);
		void BuildConstantBuffer();
		void ClearLightData();

	public:
		// 상수 버퍼들을 저장하는 벡터
		std::vector< ConstantBufferBase* > _constantBuffers;

		// 상수 버퍼 데이터를 추가하는 함수
		template <typename T>
		void CreateConstantBuffer(T* cbData, unsigned int size)
		{
			ConstantBufferBase* tCBuffer = new ConstantBuffer<T>(cbData, size);
			_constantBuffers.emplace_back(tCBuffer);
		}

	private:
		std::vector<Pg::Data::Structs::DirectionalLight> _directionalLight;
		std::vector<Pg::Data::Structs::PointLight> _pointLight;
		std::vector<Pg::Data::Structs::SpotLight> _spotLight;

	private:
		LowDX11Storage* _DXStorage;
	};
}
