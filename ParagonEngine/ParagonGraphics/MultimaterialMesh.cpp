#include "MultimaterialMesh.h"
#include "Asset3DModelData.h"
#include "GraphicsResourceManager.h"
#include "LayoutDefine.h"
#include "LowDX11Storage.h"
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"

namespace Pg::Graphics
{
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	MultimaterialMesh::MultimaterialMesh()
	{
		//고정된 File Path ( == AssetManager에서 이미 로딩된 경로가 있어야 작동하므로, 하드코딩했음.)
		_filePath = "../Resources/3DModels/TexturedMultiCubes/TexturedMultiCubeMultiMesh.fbx";

		auto tModelData = GraphicsResourceManager::Instance()->GetResource(_filePath, eAssetDefine::_3DMODEL);
		_modelData = static_cast<Asset3DModelData*>(tModelData.get());
	}

	MultimaterialMesh::~MultimaterialMesh()
	{

	}

	void MultimaterialMesh::Render()
	{
		//렌더하는 방식 : 한꺼번에 GPU에 많은 양의 메모리를 넣어놓는게 아니라,
		//DrawIndexed를 나눠서 호출하는 중에 SetResource를 다르게 사용.
		
		auto tDevice = LowDX11Storage::GetInstance()->_device;
		auto tDevCon = LowDX11Storage::GetInstance()->_deviceContext;

		auto& tD3DBuffer = _modelData->_d3dBufferInfo;
		auto& tMatCluster = _modelData->_materialCluster;

		//Input Layout 호출.
		tDevCon->IASetInputLayout(LayoutDefine::GetStatic1stLayout());

		//Constant Buffer 설정.

		//현재 Mesh가 어떤 Material을 가지고 있는지 확인해야. 
		//이에 따라 판단을 내려야 하기에.
		//나중에는 결국 Mesh Sorting 등등이 어느 정도 이루어져 있어야 할것. 

		





	}

}