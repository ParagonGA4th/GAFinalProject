#include "RendererChangeList.h"
#include "Component.h"
#include "BaseRenderer.h"

namespace Pg::Data
{
	RendererChangeList::RendererChangeList()
	{
		_onAddedRendererList.reserve(10);
		_onDeletedRendererList.reserve(10);

		ClearRendererChangeLists();
	}

	void RendererChangeList::InsertOnAddedRendererList(BaseRenderer* renderer)
	{
		_onAddedRendererList.emplace_back(renderer);
	}

	void RendererChangeList::InsertOnDeletedRendererList(BaseRenderer* renderer)
	{
		_onDeletedRendererList.emplace_back(renderer);
	}

	std::vector<BaseRenderer*>& RendererChangeList::GetAddedRendererList()
	{
		return _onAddedRendererList;
	}

	std::vector<BaseRenderer*>& RendererChangeList::GetDeletedRendererList()
	{
		return _onDeletedRendererList;
	}

	bool RendererChangeList::IfShouldUpdate()
	{
		return (!_onAddedRendererList.empty()) && (!_onDeletedRendererList.empty());
	}

	void RendererChangeList::ClearRendererChangeLists()
	{
		_onAddedRendererList.clear();
		_onDeletedRendererList.clear();
	}


}



