#pragma once


class ISerializable
{
public:
	// Scene Data -> XML
	virtual void OnSerialize() abstract;
	// XML -> Scene Data
	virtual void OnDeserialize() abstract;
	// Scene Data -> Editor Data
	virtual void OnDataStructure() abstract;
};

