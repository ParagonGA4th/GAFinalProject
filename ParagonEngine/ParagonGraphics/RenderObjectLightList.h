//#pragma once
//#include "../ParagonMath/PgMath.h"
//#include "ConstantBuffer.h"
//
//#include "LightStructs.h"
//
//namespace Pg::API::Input
//{
//	class PgInput;
//}
//
//namespace Pg::Graphics
//{
//	class RenderObjectLight;
//}
//
//namespace Pg::Data
//{
//	class Light;
//	class Transform;
//	class CameraData;
//}
//
//namespace Pg::Graphics
//{
//	struct RenderObjectLightList
//	{
//
//	public:
//		RenderObjectLightList();
//
//
//	/// 조명 리스트 관련
//	public:
//		void ClearLightData();
//		void ParseLights(Pg::Data::Transform* transform, Pg::Data::Light* lightComponent);
//
//	private:
//		// 광원을 종류별로 저장한다
//		std::vector<Pg::Data::Structs::DirectionalLight> _directionalLight;
//		std::vector<Pg::Data::Structs::PointLight> _pointLight;
//		std::vector<Pg::Data::Structs::SpotLight> _spotLight;
//		Pg::Data::Structs::LightingData _lightingData;
//
//	/// 상수 버퍼 관련
//	public:
//		// 상수 버퍼들을 저장하는 벡터
//		std::vector< ConstantBufferBase* > _constantBuffers;
//
//	public:
//		// 상수 버퍼 데이터를 추가하는 함수
//		template <typename T>
//		void CreateConstantBuffer(T* cbData, unsigned int size)
//		{
//			ConstantBufferBase* tCBuffer = new ConstantBuffer<T>(cbData, size);
//			_constantBuffers.emplace_back(tCBuffer);
//		}
//
//		void BuildConstantBuffers();
//
//
//	/// 기타
//	public:
//		void Update(Pg::Data::CameraData* camData);
//	private:
//		LowDX11Storage* _DXStorage;
//
//		Pg::API::Input::PgInput* _input;
//	};
//}
