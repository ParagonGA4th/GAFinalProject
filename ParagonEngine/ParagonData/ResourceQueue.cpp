#include "ResourceQueue.h"
#include "../ParagonHelper/ResourceHelper.h"

namespace Pg::Data
{
	void ResourceQueue::AppendResourcePair(const std::string& filePath, Pg::Data::Enums::eAssetDefine define)
	{
		_resourceQueue.push(std::make_pair(filePath, define));
	}

	bool ResourceQueue::GetPopResourcePair(std::string& outFilePath, Pg::Data::Enums::eAssetDefine& outDefine)
	{
		if (_resourceQueue.empty())
		{
			return false;
		}
		else
		{
			ResourceDefinePair tToReturn = _resourceQueue.front();
			_resourceQueue.pop();

			outFilePath = tToReturn.first;
			outDefine = tToReturn.second;

			return true;
		}
	}

	bool ResourceQueue::IsEmpty()
	{
		return _resourceQueue.empty();
	}

	void ResourceQueue::Clear()
	{
		if (_resourceQueue.empty())
		{
			return;
		}
		
		while (!_resourceQueue.empty())
		{
			_resourceQueue.pop();
		}
	}



}