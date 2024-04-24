#include "RenderObject3DList.h"

namespace Pg::Graphics
{


	std::vector<RenderObject3D*> RenderObject3DList::GetRenderObjectWithGameObject(Pg::Data::GameObject* obj)
	{
		std::vector<RenderObject3D*> tRet;

		for (auto& it : this->_staticList)
		{
			for (auto& [go, ro] : *(it.second.get()))
			{
				if (go == obj)
				{
					tRet.push_back(ro.get());
				}
			}
		}

		for (auto& it : this->_skinnedList)
		{
			for (auto& [go, ro] : *(it.second.get()))
			{
				if (go == obj)
				{
					tRet.push_back(ro.get());
				}
			}
		}

		for (auto& it : this->_allAlphaBlendedList)
		{
			if (it._obj == obj)
			{
				if (it._isSkinned)
				{
					tRet.push_back(it._eitherSkinnedMesh.get());
				}
				else
				{
					tRet.push_back(it._eitherStaticMesh.get());
				}
			}
		}

		return tRet;
	}

	void RenderObject3DList::DeleteRenderObjectWithGameObject(Pg::Data::GameObject* obj)
	{
		//없다면 점검할 필요 조차 없다.

		//하나하나 vector.
		if (!_staticList.empty())
		{
			for (auto& [bMat, bVec] : this->_staticList)
			{
				auto tVec = bVec.get();
				if (!tVec->empty())
				{
					tVec->erase(std::remove_if(tVec->begin(), tVec->end(), [&obj](auto& tPair) { return tPair.first == obj; }));
				}
			}
		}

		if (!_skinnedList.empty())
		{
			for (auto& [bMat, bVec] : this->_skinnedList)
			{
				auto tVec = bVec.get();
				if (!tVec->empty())
				{
					tVec->erase(std::remove_if(tVec->begin(), tVec->end(), [&obj](auto& tPair) { return tPair.first == obj; }));
				}
			}
		}

		if (!_allAlphaBlendedList.empty())
		{
			for (auto& it : this->_allAlphaBlendedList)
			{
				if (it._obj == obj)
				{
					it._eitherStaticMesh.reset();
					it._eitherSkinnedMesh.reset();
				}
			}
		}
	}

	void RenderObject3DList::SortBlendedByDepth_BackToFront(Pg::Data::CameraData* camData)
	{
		//일단 카메라를 상대적으로 있는 
		
	}

}