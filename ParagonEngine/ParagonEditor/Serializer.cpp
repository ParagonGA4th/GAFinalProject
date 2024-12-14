#include "Serializer.h"

void Pg::Serialize::Serializer::Serialize(std::string typeName, pugi::xml_node* node, const std::string& name, void* result)
{
	std::variant<bool, int, float, double, std::string,
		Pg::Math::PGFLOAT2, Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT4, Pg::Math::PGQuaternion,
		Pg::Data::Model, Pg::Data::Texture, Pg::Data::Material, Pg::Data::Sound> vunion;

	if (typeName == typeid(bool).name())
	{
		vunion = *(static_cast<bool*>(result));
		auto tRes = std::get<bool>(vunion);
		SerializeBoolean(node, name, tRes);
	}
	if (typeName == typeid(int).name())
	{
		vunion = *(static_cast<int*>(result));
		auto tRes = std::get<int>(vunion);
		SerializeInt(node, name, tRes);
	}
	if (typeName == typeid(float).name())
	{
		vunion = *(static_cast<float*>(result));
		auto tRes = std::get<float>(vunion);
		SerializeFloat(node, name, tRes);
	}
	if (typeName == typeid(double).name())
	{
		vunion = *(static_cast<double*>(result));
		auto tRes = std::get<double>(vunion);
		SerializeDouble(node, name, tRes);
	}
	if (typeName == typeid(std::string).name())
	{
		vunion = *(static_cast<std::string*>(result));
		auto tRes = std::get<std::string>(vunion);
		SerializeString(node, name, tRes);
	}
	if (typeName == typeid(Pg::Math::PGFLOAT2).name())
	{
		vunion = *(static_cast<Pg::Math::PGFLOAT2*>(result));
		auto tRes = std::get<Pg::Math::PGFLOAT2>(vunion);
		SerializePGFloat2(node, name, tRes);
	}
	if (typeName == typeid(Pg::Math::PGFLOAT3).name())
	{
		vunion = *(static_cast<Pg::Math::PGFLOAT3*>(result));
		auto tRes = std::get<Pg::Math::PGFLOAT3>(vunion);
		SerializePGFloat3(node, name, tRes);
	}
	if (typeName == typeid(Pg::Math::PGFLOAT4).name())
	{
		vunion = *(static_cast<Pg::Math::PGFLOAT4*>(result));
		auto tRes = std::get<Pg::Math::PGFLOAT4>(vunion);
		SerializePGFloat4(node, name, tRes);
	}
	if (typeName == typeid(Pg::Math::PGQuaternion).name())
	{
		vunion = *(static_cast<Pg::Math::PGQuaternion*>(result));
		auto tRes = std::get<Pg::Math::PGQuaternion>(vunion);
		SerializePGQuat(node, name, tRes);
	}
	if (typeName == typeid(Pg::Data::Model).name())
	{
		vunion = *(static_cast<Pg::Data::Model*>(result));
		auto tRes = std::get<Pg::Data::Model>(vunion);
		SerializeString(node, name, tRes.path);
	}
	if (typeName == typeid(Pg::Data::Texture).name())
	{
		vunion = *(static_cast<Pg::Data::Texture*>(result));
		auto tRes = std::get<Pg::Data::Texture>(vunion);
		SerializeString(node, name, tRes.path);
	}
	if (typeName == typeid(Pg::Data::Material).name())
	{
		vunion = *(static_cast<Pg::Data::Material*>(result));
		auto tRes = std::get<Pg::Data::Material>(vunion);
		SerializeString(node, name, tRes.path);
	}
	if (typeName == typeid(Pg::Data::Sound).name())
	{
		vunion = *(static_cast<Pg::Data::Sound*>(result));
		auto tRes = std::get<Pg::Data::Sound>(vunion);
		SerializeString(node, name, tRes.path);
	}
}

void Pg::Serialize::Serializer::SerializeUint(pugi::xml_node* node, const std::string& name, uint32_t val)
{
	pugi::xml_node uintNode = node->append_child(name.c_str());
	uintNode.text().set(std::to_string(val).c_str());
}

void Pg::Serialize::Serializer::SerializeBoolean(pugi::xml_node* node, const std::string& name, bool val)
{
	pugi::xml_node boolNode = node->append_child(name.c_str());
	boolNode.text().set(val ? "true" : "false");
}

void Pg::Serialize::Serializer::SerializeInt(pugi::xml_node* node, const std::string& name, int val)
{
	pugi::xml_node intNode = node->append_child(name.c_str());
	intNode.text().set(std::to_string(val).c_str());
}

void Pg::Serialize::Serializer::SerializeFloat(pugi::xml_node* node, const std::string& name, float val)
{
	pugi::xml_node floatNode = node->append_child(name.c_str());
	floatNode.text().set(std::to_string(val).c_str());
}

void Pg::Serialize::Serializer::SerializeDouble(pugi::xml_node* node, const std::string& name, double val)
{
	pugi::xml_node doubletNode = node->append_child(name.c_str());
	doubletNode.text().set(std::to_string(val).c_str());
}

void Pg::Serialize::Serializer::SerializeString(pugi::xml_node* node, const std::string& name, std::string val)
{
	pugi::xml_node stringNode = node->append_child(name.c_str());
	stringNode.text().set(val.c_str());
}

void Pg::Serialize::Serializer::SerializePGFloat2(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT2 val)
{
	pugi::xml_node vec2Node = node->append_child(name.c_str());

	pugi::xml_node child1 = vec2Node.append_child("x");
	child1.text().set(std::to_string(val.x).c_str());
	pugi::xml_node child2 = vec2Node.append_child("y");
	child2.text().set(std::to_string(val.y).c_str());
}

void Pg::Serialize::Serializer::SerializePGFloat3(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT3 val)
{
	pugi::xml_node vec3Node = node->append_child(name.c_str());

	pugi::xml_node child1 = vec3Node.append_child("x");
	child1.text().set(std::to_string(val.x).c_str());
	pugi::xml_node child2 = vec3Node.append_child("y");
	child2.text().set(std::to_string(val.y).c_str());
	pugi::xml_node child3 = vec3Node.append_child("z");
	child3.text().set(std::to_string(val.z).c_str());
}

void Pg::Serialize::Serializer::SerializePGFloat4(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT4 val)
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

void Pg::Serialize::Serializer::SerializePGQuat(pugi::xml_node* node, const std::string& name, Pg::Math::PGQuaternion val)
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

void Pg::Serialize::Serializer::SerializeColor(pugi::xml_node* node, const std::string& name, Pg::Math::PGFLOAT4 val)
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

void Pg::Serialize::Serializer::Deserialize(std::string typeName, pugi::xml_node* node, void* result)
{
	std::variant<bool, int, float, double, std::string, unsigned, int64_t,
		Pg::Math::PGFLOAT2, Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT4, Pg::Math::PGQuaternion> vunion;

	if (typeName == typeid(bool).name())
	{
		vunion = DeserializeBoolean(node, "");
		auto tRes = std::get<bool>(vunion);
		memcpy(result, &tRes, sizeof(bool));
	}
	else if (typeName == typeid(int).name())
	{
		vunion = DeserializeInt(node, "");
		auto tRes = std::get<int>(vunion);
		memcpy(result, &tRes, sizeof(int));
	}
	else if (typeName == typeid(float).name())
	{
		vunion = DeserializeFloat(node, "");
		auto tRes = std::get<float>(vunion);
		memcpy(result, &tRes, sizeof(float));
	}
	else if (typeName == typeid(double).name())
	{
		vunion = DeserializeDouble(node, "");
		auto tRes = std::get<double>(vunion);
		memcpy(result, &tRes, sizeof(double));
	}
	else if (typeName == typeid(std::string).name())
	{
		vunion = DeserializeString(node, "");
		auto tRes = std::get<std::string>(vunion);
		//memcpy(result, &tRes, sizeof(std::string));
		std::string* tResultStr = static_cast<std::string*>(result);
		*tResultStr = tRes;
	}
	else if (typeName == typeid(unsigned).name())
	{
		vunion = DeserializeUint(node, "");
		auto tRes = std::get<unsigned>(vunion);
		memcpy(result, &tRes, sizeof(unsigned));
	}
	else if (typeName == typeid(int64_t).name())
	{
		vunion = DeserializeInt64(node, "");
		auto tRes = std::get<int64_t>(vunion);
		memcpy(result, &tRes, sizeof(int64_t));
	}
	else if (typeName == typeid(Pg::Math::PGFLOAT2).name())
	{
		vunion = DeserializePGFloat2(node);
		auto tRes = std::get<Pg::Math::PGFLOAT2>(vunion);
		memcpy(result, &tRes, sizeof(Pg::Math::PGFLOAT2));
	}
	else if (typeName == typeid(Pg::Math::PGFLOAT3).name())
	{
		vunion = DeserializePGFloat3(node);
		auto tRes = std::get<Pg::Math::PGFLOAT3>(vunion);
		memcpy(result, &tRes, sizeof(Pg::Math::PGFLOAT3));
	}
	else if (typeName == typeid(Pg::Math::PGFLOAT4).name())
	{
		vunion = DeserializePGFloat4(node);
		auto tRes = std::get<Pg::Math::PGFLOAT4>(vunion);
		memcpy(result, &tRes, sizeof(Pg::Math::PGFLOAT4));
	}
	else if (typeName == typeid(Pg::Math::PGQuaternion).name())
	{
		vunion = DeserializePGQuaternion(node);
		auto tRes = std::get<Pg::Math::PGQuaternion>(vunion);
		memcpy(result, &tRes, sizeof(Pg::Math::PGQuaternion));
	}
}


void Pg::Serialize::Serializer::DeserializeBoolean(pugi::xml_node* node, const std::string& name, bool& result)
{
	if (name.empty())
	{
		result = node->text().as_bool();
	}
	else
	{
		// 이름으로 해당 노드값이 존재하는지 가져온다
		pugi::xml_node nodeName = node->child(name.c_str());

		if (nodeName)
		{
			// 해당 노드값에 존재하는 string 값을 가져와 비교한다
			result = nodeName.text().as_bool();
		}
	}
}

bool Pg::Serialize::Serializer::DeserializeBoolean(pugi::xml_node* node, const std::string& name)
{
	if (name.empty())
	{
		return node->text().as_bool();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			return nodeName.text().as_bool();
		}
		else
		{
			return false;
		}
	}
}

void Pg::Serialize::Serializer::DeserializeInt(pugi::xml_node* node, const std::string& name, int& result)
{
	if (name.empty())
	{
		result = node->text().as_int();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result = nodeName.text().as_int();
		}
	}
}

int Pg::Serialize::Serializer::DeserializeInt(pugi::xml_node* node, const std::string& name)
{
	if (name.empty())
	{
		return node->text().as_int();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			return nodeName.text().as_int();
		}
	}
}

void Pg::Serialize::Serializer::DeserializeFloat(pugi::xml_node* node, const std::string& name, float& result)
{
	if (name.empty())
	{
		result = node->text().as_float();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result = nodeName.text().as_float();
		}
	}
}

float Pg::Serialize::Serializer::DeserializeFloat(pugi::xml_node* node, const std::string& name)
{
	if (name.empty())
	{
		return node->text().as_float();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			return nodeName.text().as_float();
		}
	}
}

void Pg::Serialize::Serializer::DeserializeDouble(pugi::xml_node* node, const std::string& name, double& result)
{
	if (name.empty())
	{
		result = node->text().as_double();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result = nodeName.text().as_double();
		}
	}
}

double Pg::Serialize::Serializer::DeserializeDouble(pugi::xml_node* node, const std::string& name)
{
	if (name.empty())
	{
		return node->text().as_double();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			return nodeName.text().as_double();
		}
	}
}


void Pg::Serialize::Serializer::DeserializeString(pugi::xml_node* node, const std::string& name, std::string& result)
{
	if (name.empty())
	{
		result = node->text().as_string();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());

		if (nodeName)
		{
			result = nodeName.text().as_string();
		}
	}
}

std::string Pg::Serialize::Serializer::DeserializeString(pugi::xml_node* node, const std::string& name)
{
	if (name.empty())
	{
		return node->text().as_string();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());

		if (nodeName == NULL)
		{
			return "";
		}

		if (nodeName)
		{
			return nodeName.text().as_string();
		}
	}
}

void Pg::Serialize::Serializer::DeserializeUint(pugi::xml_node* node, const std::string& name, uint32_t& result)
{
	if (name.empty())
	{
		result = node->text().as_uint();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result = nodeName.text().as_uint();
		}
	}
}

unsigned Pg::Serialize::Serializer::DeserializeUint(pugi::xml_node* node, const std::string& name)
{
	if (name.empty())
	{
		return node->text().as_uint();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			return nodeName.text().as_uint();
		}
	}
}

void Pg::Serialize::Serializer::DeserializeInt64(pugi::xml_node* node, const std::string& name, int64_t& result)
{
	if (name.empty())
	{
		result = node->text().as_int();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			result = nodeName.text().as_int();
		}
	}
}

int64_t Pg::Serialize::Serializer::DeserializeInt64(pugi::xml_node* node, const std::string& name)
{
	if (name.empty())
	{
		return node->text().as_int();
	}
	else
	{
		pugi::xml_node nodeName = node->child(name.c_str());
		if (nodeName)
		{
			return nodeName.text().as_int();
		}
	}
}

void Pg::Serialize::Serializer::DeserializePGFloat2(pugi::xml_node* node, Pg::Math::PGFLOAT2& result)
{
	pugi::xml_node nodeName = node->child("x");
	if (nodeName)
	{
		result.x = nodeName.first_child().text().as_float();
		result.y = nodeName.next_sibling().first_child().text().as_float();
	}
}

Pg::Math::PGFLOAT2 Pg::Serialize::Serializer::DeserializePGFloat2(pugi::xml_node* node)
{
	pugi::xml_node nodeName = node->child("x");
	if (nodeName)
	{
		Pg::Math::PGFLOAT2 result;
		result.x = nodeName.first_child().text().as_float();
		result.y = nodeName.next_sibling().first_child().text().as_float();
		return result;
	}
}

void Pg::Serialize::Serializer::DeserializePGFloat3(pugi::xml_node* node, Pg::Math::PGFLOAT3& result)
{
	pugi::xml_node nodeName = node->child("x");
	if (nodeName)
	{
		result.x = nodeName.first_child().text().as_float();
		result.y = nodeName.next_sibling().first_child().text().as_float();
		result.z = nodeName.next_sibling().next_sibling().first_child().text().as_float();
	}
}

Pg::Math::PGFLOAT3 Pg::Serialize::Serializer::DeserializePGFloat3(pugi::xml_node* node)
{
	pugi::xml_node nodeName = node->child("x");
	Pg::Math::PGFLOAT3 result;
	if (nodeName)
	{
		result.x = nodeName.first_child().text().as_float();
		result.y = nodeName.next_sibling().first_child().text().as_float();
		result.z = nodeName.next_sibling().next_sibling().first_child().text().as_float();
	}
	else
	{
		result.x = 0.f;
		result.y = 0.f;
		result.z = 0.f;
	}
	return result;
}


void Pg::Serialize::Serializer::DeserializePGFloat4(pugi::xml_node* node, Pg::Math::PGFLOAT4& result)
{
	pugi::xml_node nodeName = node->child("w");
	if (nodeName)
	{
		result.w = nodeName.first_child().text().as_float();
		result.x = nodeName.next_sibling().first_child().text().as_float();
		result.y = nodeName.next_sibling().next_sibling().first_child().text().as_float();
		result.z = nodeName.next_sibling().next_sibling().next_sibling().first_child().text().as_float();
	}
}

Pg::Math::PGFLOAT4 Pg::Serialize::Serializer::DeserializePGFloat4(pugi::xml_node* node)
{
	pugi::xml_node nodeName = node->child("w");
	Pg::Math::PGFLOAT4 result;
	if (nodeName)
	{
		result.w = nodeName.first_child().text().as_float();
		result.x = nodeName.next_sibling().first_child().text().as_float();
		result.y = nodeName.next_sibling().next_sibling().first_child().text().as_float();
		result.z = nodeName.next_sibling().next_sibling().next_sibling().first_child().text().as_float();
	}
	else
	{
		result.w = 1.0f;
		result.x = 0.f;
		result.y = 0.f;
		result.z = 0.f;
	}
	return result;
}

void Pg::Serialize::Serializer::DeserializePGQuaternion(pugi::xml_node* node, Pg::Math::PGQuaternion& result)
{
	pugi::xml_node nodeName = node->child("w");
	if (nodeName)
	{
		result.w = nodeName.first_child().text().as_float();
		result.x = nodeName.next_sibling().first_child().text().as_float();
		result.y = nodeName.next_sibling().next_sibling().first_child().text().as_float();
		result.z = nodeName.next_sibling().next_sibling().next_sibling().first_child().text().as_float();
	}
}

Pg::Math::PGQuaternion Pg::Serialize::Serializer::DeserializePGQuaternion(pugi::xml_node* node)
{
	pugi::xml_node nodeName = node->child("w");
		Pg::Math::PGQuaternion result;
	if (nodeName)
	{
		result.w = nodeName.first_child().text().as_float();
		result.x = nodeName.next_sibling().first_child().text().as_float();
		result.y = nodeName.next_sibling().next_sibling().first_child().text().as_float();
		result.z = nodeName.next_sibling().next_sibling().next_sibling().first_child().text().as_float();
	}
	else
	{
		result.w = 1.0f;
		result.x = 0.f;
		result.y = 0.f;
		result.z = 0.f;
	}
	return result;
}