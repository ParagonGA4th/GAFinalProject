#pragma once
#include "../ParagonUtil/pugixml.hpp"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/ModelData.h"
#include "../ParagonData/SoundData.h"

namespace Pg::Serialize
{
	class Serializer
	{
	public:
		// Serialize
		static void SerializeBoolean(pugi::xml_node* node, const std::string& name, bool val);
		static void SerializeString(pugi::xml_node* node, const std::string& name, std::string val);
		static void SerializeFloat(pugi::xml_node* node, const std::string& name, float val);
		static void SerialzieDouble(pugi::xml_node* node, const std::string& name, double val);
		static void SerializeInt(pugi::xml_node* node, const std::string& name, int val);
		static void SerializeVector2(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT2 val);
		static void SerializeVector3(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT3 val);
		static void SerializeVector4(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT4 val);
		static void SerializeQuat(pugi::xml_node* node, const std::string& name, Pg::Math::PGQuaternion val);
		static void SerializeColor(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT4 val);
		static void SerializeModelPath(pugi::xml_node* node, const std::string& name, Pg::Data::Model val);
		static void SerializeTexturePath(pugi::xml_node* node, const std::string& name, Pg::Data::Texture val);
		static void SerializeMaterialPath(pugi::xml_node* node, const std::string& name, Pg::Data::Material val);
		static void SerializeSoundPath(pugi::xml_node* node, const std::string& name, Pg::Data::Sound val);
		

		// Deserialize
		static void DeserializeBoolean(pugi::xml_node* node, const std::string& name, bool& result);
		static bool DeserializeBoolean(pugi::xml_node* node, const std::string& name);
		
		static void DeserializeString(pugi::xml_node* node, const std::string& name, std::string& result);
		static std::string DeserializeString(pugi::xml_node* node, const std::string& name);

		static void DeserializeFloat(pugi::xml_node* node, const std::string& name, float& result);
		static float DeserializeFloat(pugi::xml_node* node, const std::string& name);

		static void DeserializeDouble(pugi::xml_node* node, const std::string& name, double& result);
		static double DeserializeDouble(pugi::xml_node* node, const std::string& name);

		static void DeserializeInt(pugi::xml_node* node, const std::string& name, int& result);
		static bool DeserializeInt(pugi::xml_node* node, const std::string& name);

		static void DeserializeUint(pugi::xml_node* node, const std::string& name, unsigned& result);
		static unsigned DeserializeUint(pugi::xml_node* node, const std::string& name);

		static void DeserializeInt64(pugi::xml_node* node, const std::string& name, int64_t& result);
		static int64_t DeserializeInt64(pugi::xml_node* node, const std::string& name);

		static void DeserializeVec2(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT2& result);
		static Pg::Math::PGFLOAT2 DeserializeVec2(pugi::xml_node* node, const std::string& name);

		static void DeserializeVec3(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT3& result);
		static Pg::Math::PGFLOAT3 DeserializeVec3(pugi::xml_node* node, const std::string& name);

		static void DeserializeVec4(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT4& result);
		static Pg::Math::PGFLOAT4 DeserializeVec4(pugi::xml_node* node, const std::string& name);

		static void DeserializeQuaternion(pugi::xml_node* node, const std::string& name, Pg::Math::PGQuaternion& result);
		static Pg::Math::PGQuaternion DeserializeQuaternion(pugi::xml_node* node, const std::string& name);


	};

}


