#pragma once
#include "../ParagonUtil/pugixml.hpp"

class ISerializable
{
public:
	// Scene Data -> XML
	virtual void OnSerialize() = 0;
	// XML -> Scene Data
	virtual void OnDeserialize() = 0;
	// Scene Data -> Editor Data
	virtual void OnDataStructure() = 0;
};

