#include "ArtifactBox.h"

namespace Pg::DataScript
{
	ArtifactBox::ArtifactBox(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{
	}

	void ArtifactBox::BeforePhysicsAwake()
	{
	}

	void ArtifactBox::Awake()
	{
	}

	void ArtifactBox::Update()
	{
	}

	void ArtifactBox::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
	}

	void ArtifactBox::OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count)
	{
	}
}