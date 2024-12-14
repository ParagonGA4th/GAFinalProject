#pragma once
#include "../ParagonHelper/pugixml.hpp"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/ModelData.h"
#include "../ParagonData/SoundData.h"

#include <variant>


using var_t = std::variant<bool, int, float, double, std::string, unsigned, int64_t,
	Pg::Math::PGFLOAT2, Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT4, Pg::Math::PGQuaternion>;

namespace Pg::Serialize
{
	class Serializer
	{
	public:
		// Serialize

		/// <summary>
		/// TypeSerialize
		/// </summary>
		/// <param name="typeName">TypeNmae</param>
		/// <param name="node">데이터를 저장할 노드</param>
		/// <param name="name">노드명</param>
		/// <param name="result">노드의 데이터가 될 포인터</param>
		static void Serialize(std::string typeName, pugi::xml_node* node, const std::string& name, void* result);
		static void SerializeUint(pugi::xml_node* node, const std::string& name, uint32_t val);
		static void SerializeBoolean(pugi::xml_node* node, const std::string& name, bool val);
		static void SerializeInt(pugi::xml_node* node, const std::string& name, int val);
		static void SerializeFloat(pugi::xml_node* node, const std::string& name, float val);
		static void SerializeDouble(pugi::xml_node* node, const std::string& name, double val);
		static void SerializeString(pugi::xml_node* node, const std::string& name, std::string val);
		static void SerializePGFloat2(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT2 val);
		static void SerializePGFloat3(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT3 val);
		static void SerializePGFloat4(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT4 val);
		static void SerializePGQuat(pugi::xml_node* node, const std::string& name, Pg::Math::PGQuaternion val);
		static void SerializeColor(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT4 val);		

		// Deserialize

		/// <summary>
		/// Type Deserialize
		/// </summary>
		/// <param name="typeName">TypeName</param>
		/// <param name="node">데이터를 가져올 노드</param>
		/// <param name="result">데이터를 담을 포인터</param>
		static void Deserialize(std::string typeName, pugi::xml_node* node, void* result);
		
		static void DeserializeBoolean(pugi::xml_node* node, const std::string& name, bool& result);
		static bool DeserializeBoolean(pugi::xml_node* node, const std::string& name);
		
		static void DeserializeInt(pugi::xml_node* node, const std::string& name, int& result);
		static int DeserializeInt(pugi::xml_node* node, const std::string& name);

		static void DeserializeFloat(pugi::xml_node* node, const std::string& name, float& result);
		static float DeserializeFloat(pugi::xml_node* node, const std::string& name);

		static void DeserializeDouble(pugi::xml_node* node, const std::string& name, double& result);
		static double DeserializeDouble(pugi::xml_node* node, const std::string& name);

		static void DeserializeString(pugi::xml_node* node, const std::string& name, std::string& result);
		static std::string DeserializeString(pugi::xml_node* node, const std::string& name);

		static void DeserializeUint(pugi::xml_node* node, const std::string& name, uint32_t& result);
		static uint32_t DeserializeUint(pugi::xml_node* node, const std::string& name);

		static void DeserializeInt64(pugi::xml_node* node, const std::string& name, int64_t& result);
		static int64_t DeserializeInt64(pugi::xml_node* node, const std::string& name);

		static void DeserializePGFloat2(pugi::xml_node* node, Pg::Math::PGFLOAT2& result);
		static Pg::Math::PGFLOAT2 DeserializePGFloat2(pugi::xml_node* node);

		static void DeserializePGFloat3(pugi::xml_node* node, Pg::Math::PGFLOAT3& result);
		static Pg::Math::PGFLOAT3 DeserializePGFloat3(pugi::xml_node* node);

		static void DeserializePGFloat4(pugi::xml_node* node, Pg::Math::PGFLOAT4& result);
		static Pg::Math::PGFLOAT4 DeserializePGFloat4(pugi::xml_node* node);

		static void DeserializePGQuaternion(pugi::xml_node* node, Pg::Math::PGQuaternion& result);
		static Pg::Math::PGQuaternion DeserializePGQuaternion(pugi::xml_node* node);
	};
}


