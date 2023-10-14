#pragma once
#include <string>

/// <summary>
/// Deferred Rendering이 전역적으로 자리잡기 전에, 
/// 다중 Material 적용되는 Mesh 렌더 데모를 위해.
/// </summary>

namespace Pg::Graphics
{
	class Asset3DModelData;
}

namespace Pg::Graphics
{
	class MultimaterialMesh
	{
	public:
		MultimaterialMesh();
		~MultimaterialMesh();

		//단순한 Mesh Rendering.
		void Render();


	private:
		std::string _filePath;
		Asset3DModelData* _modelData = nullptr;
	};
}


