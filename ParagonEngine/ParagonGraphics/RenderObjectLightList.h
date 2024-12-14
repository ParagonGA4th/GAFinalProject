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
//	/// ���� ����Ʈ ����
//	public:
//		void ClearLightData();
//		void ParseLights(Pg::Data::Transform* transform, Pg::Data::Light* lightComponent);
//
//	private:
//		// ������ �������� �����Ѵ�
//		std::vector<Pg::Data::Structs::DirectionalLight> _directionalLight;
//		std::vector<Pg::Data::Structs::PointLight> _pointLight;
//		std::vector<Pg::Data::Structs::SpotLight> _spotLight;
//		Pg::Data::Structs::LightingData _lightingData;
//
//	/// ��� ���� ����
//	public:
//		// ��� ���۵��� �����ϴ� ����
//		std::vector< ConstantBufferBase* > _constantBuffers;
//
//	public:
//		// ��� ���� �����͸� �߰��ϴ� �Լ�
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
//	/// ��Ÿ
//	public:
//		void Update(Pg::Data::CameraData* camData);
//	private:
//		LowDX11Storage* _DXStorage;
//
//		Pg::API::Input::PgInput* _input;
//	};
//}
