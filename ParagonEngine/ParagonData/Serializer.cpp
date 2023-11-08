#include "Serializer.h"


namespace Test
{

	void Test::Serializer::SerializeBoolean(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, bool val)
	{
		pugi::xml_node boolNode = node->append_child(name.c_str());
		boolNode.text().set(val ? "true" : "false");
	}


	void Serializer::SerializeString(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, std::string val)
	{
		pugi::xml_node stringNode = node->append_child(name.c_str());
		stringNode.text().set(val.c_str());
	}

	void Serializer::SerializeFloat(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, float val)
	{
		pugi::xml_node floatNode = node->append_child(name.c_str());
		floatNode.text().set(std::to_string(val).c_str());
	}

	void Serializer::SerialzieDouble(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, double val)
	{
		pugi::xml_node doubletNode = node->append_child(name.c_str());
		doubletNode.text().set(std::to_string(val).c_str());
	}

	void Serializer::SerializeInt(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, int val)
	{
		pugi::xml_node intNode = node->append_child(name.c_str());
		intNode.text().set(std::to_string(val).c_str());
	}

	void Serializer::SerializeVector2(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT2 val)
	{
		pugi::xml_node vec2Node = node->append_child(name.c_str());

		pugi::xml_node child1 = vec2Node.append_child("x");
		child1.text().set(std::to_string(val.x).c_str());
		pugi::xml_node child2 = vec2Node.append_child("y");
		child2.text().set(std::to_string(val.y).c_str());
	}

	void Serializer::SerializeVector3(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT3 val)
	{
		pugi::xml_node vec3Node = node->append_child(name.c_str());

		pugi::xml_node child1 = vec3Node.append_child("x");
		child1.text().set(std::to_string(val.x).c_str());
		pugi::xml_node child2 = vec3Node.append_child("y");
		child2.text().set(std::to_string(val.y).c_str());
		pugi::xml_node child3 = vec3Node.append_child("z");
		child3.text().set(std::to_string(val.z).c_str());
	}

	void Serializer::SerializeVector4(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT4 val)
	{
		pugi::xml_node vec4Node = node->append_child(name.c_str());

		pugi::xml_node x = vec4Node.append_child("x");
		x.text().set(std::to_string(val.x).c_str());
		pugi::xml_node y = vec4Node.append_child("y");
		y.text().set(std::to_string(val.y).c_str());
		pugi::xml_node z = vec4Node.append_child("z");
		z.text().set(std::to_string(val.z).c_str());
		pugi::xml_node w = vec4Node.append_child("w");
		w.text().set(std::to_string(val.w).c_str());
	}

	void Serializer::SerializeQuat(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, Pg::Math::PGQuaternion val)
	{
		pugi::xml_node vec4Node = node->append_child(name.c_str());

		pugi::xml_node w = vec4Node.append_child("w");
		w.text().set(std::to_string(val.w).c_str());
		pugi::xml_node x = vec4Node.append_child("x");
		x.text().set(std::to_string(val.x).c_str());
		pugi::xml_node y = vec4Node.append_child("y");
		y.text().set(std::to_string(val.y).c_str());
		pugi::xml_node z = vec4Node.append_child("z");
		z.text().set(std::to_string(val.z).c_str());
	}

	void Serializer::SerializeColor(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT4 val)
	{
		pugi::xml_node colorNode = node->append_child(name.c_str());

		pugi::xml_node r = colorNode.append_child("r");
		r.text().set(std::to_string(val.x).c_str());
		pugi::xml_node g = colorNode.append_child("g");
		g.text().set(std::to_string(val.y).c_str());
		pugi::xml_node b = colorNode.append_child("b");
		b.text().set(std::to_string(val.z).c_str());
		pugi::xml_node a = colorNode.append_child("a");
		a.text().set(std::to_string(val.w).c_str());
	}

	void Serializer::SerializeModelPath(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, Pg::Data::Model val)
	{
		SerializeString(doc, node, name, val.name);
	}

	void Serializer::SerializeTexturePath(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, Pg::Data::Texture val)
	{
		SerializeString(doc, node, name, val.name);
	}

	void Serializer::SerializeMaterialPath(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, Pg::Data::Material val)
	{
		SerializeString(doc, node, name, val.name);
	}

	void Serializer::SerializeSoundPath(pugi::xml_document& doc, pugi::xml_node* node, const std::string& name, Pg::Data::Sound val)
	{
		SerializeString(doc, node, name, val.name);
	}

	void Serializer::DeserializeBoolean(pugi::xml_node* node, const std::string& name, bool& result)
	{
		// 이름으로 해당 노드값이 존재하는지 가져온다
		pugi::xml_node nodeName = node->child(name.c_str());

		if (nodeName)
		{
			// 해당 노드값에 존재하는 string 값을 가져와 비교한다
			result = nodeName.text().as_string();
		}
		else 
		{
			// 해당 노드가 존재하지 않음
		}
	}

	bool Serializer::DeserializeBoolean(pugi::xml_node* node, const std::string& name)
	{
		pugi::xml_node nodeName = node->child(name.c_str());

		if (nodeName)
		{
			return nodeName.text().as_bool();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	void Serializer::DeserializeString(pugi::xml_node* node, const std::string& name, std::string& result)
	{
		pugi::xml_node nodeName = node->child(name.c_str());

		if (nodeName)
		{
			result = nodeName.text().as_string();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	std::string Serializer::DeserializeString(pugi::xml_node* node, const std::string& name)
	{
		pugi::xml_node nodeName = node->child(name.c_str());

		if (nodeName)
		{
			return nodeName.text().as_string();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	void Serializer::DeserializeFloat(pugi::xml_node* node, const std::string& name, float& result)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result = nodeName.text().as_float();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	float Serializer::DeserializeFloat(pugi::xml_node* node, const std::string& name)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			return nodeName.text().as_float();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	void Serializer::DeserializeDouble(pugi::xml_node* node, const std::string& name, double& result)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result = nodeName.text().as_double();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	double Serializer::DeserializeDouble(pugi::xml_node* node, const std::string& name)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			return nodeName.text().as_double();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	void Serializer::DeserializeInt(pugi::xml_node* node, const std::string& name, int& result)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result = nodeName.text().as_int();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	bool Serializer::DeserializeInt(pugi::xml_node* node, const std::string& name)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			return nodeName.text().as_int();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	void Serializer::DeserializeUint(pugi::xml_node* node, const std::string& name, unsigned& result)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result = nodeName.text().as_uint();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	unsigned Serializer::DeserializeUint(pugi::xml_node* node, const std::string& name)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			return nodeName.text().as_uint();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	void Serializer::DeserializeInt64(pugi::xml_node* node, const std::string& name, int64_t& result)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result = nodeName.text().as_int();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	int64_t Serializer::DeserializeInt64(pugi::xml_node* node, const std::string& name)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			return nodeName.text().as_int();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	void Serializer::DeserializeVec2(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT2& result)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result.x = nodeName.first_child().text().as_float();
			result.y = nodeName.first_child().next_sibling().text().as_float();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	Pg::Math::PGFLOAT2 Serializer::DeserializeVec2(pugi::xml_node* node, const std::string& name)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			Pg::Math::PGFLOAT2 result;
			result.x = nodeName.first_child().text().as_float();
			result.y = nodeName.first_child().next_sibling().text().as_float();
			return result;
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	void Serializer::DeserializeVec3(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT3& result)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result.x = nodeName.first_child().text().as_float();
			result.y = nodeName.first_child().next_sibling().text().as_float();
			result.z = nodeName.first_child().next_sibling().next_sibling().text().as_float();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	Pg::Math::PGFLOAT3 Serializer::DeserializeVec3(pugi::xml_node* node, const std::string& name)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			Pg::Math::PGFLOAT3 result;
			result.x = nodeName.first_child().text().as_float();
			result.y = nodeName.first_child().next_sibling().text().as_float();
			result.z = nodeName.first_child().next_sibling().next_sibling().text().as_float();
			return result;
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}


	void Serializer::DeserializeVec4(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT4& result)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result.x = nodeName.first_child().text().as_float();
			result.y = nodeName.first_child().next_sibling().text().as_float();
			result.z = nodeName.first_child().next_sibling().next_sibling().text().as_float();
			result.w = nodeName.first_child().next_sibling().next_sibling().next_sibling().text().as_float();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	Pg::Math::PGFLOAT4 Serializer::DeserializeVec4(pugi::xml_node* node, const std::string& name)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			Pg::Math::PGFLOAT4 result;
			result.x = nodeName.first_child().text().as_float();
			result.y = nodeName.first_child().next_sibling().text().as_float();
			result.z = nodeName.first_child().next_sibling().next_sibling().text().as_float();
			result.w = nodeName.first_child().next_sibling().next_sibling().next_sibling().text().as_float();
			return result;
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	void Serializer::DeserializeQuaternion(pugi::xml_node* node, const std::string& name, Pg::Math::PGQuaternion& result)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result.w = nodeName.first_child().text().as_float();
			result.x = nodeName.first_child().next_sibling().text().as_float();
			result.y = nodeName.first_child().next_sibling().next_sibling().text().as_float();
			result.z = nodeName.first_child().next_sibling().next_sibling().next_sibling().text().as_float();
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

	Pg::Math::PGQuaternion Serializer::DeserializeQuaternion(pugi::xml_node* node, const std::string& name)
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			Pg::Math::PGQuaternion result;
			result.w = nodeName.first_child().text().as_float();
			result.x = nodeName.first_child().next_sibling().text().as_float();
			result.y = nodeName.first_child().next_sibling().next_sibling().text().as_float();
			result.z = nodeName.first_child().next_sibling().next_sibling().next_sibling().text().as_float();
			return result;
		}
		else
		{
			// 해당 노드가 존재하지 않음
		}
	}

}
