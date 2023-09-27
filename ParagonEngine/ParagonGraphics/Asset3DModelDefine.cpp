#include "Asset3DModelDefine.h"

namespace Pg::Graphics
{
	///생성/소멸자 딴에서 메모리 할당 안 할거다!
	///일괄적으로 처리 불가능.
	///다만, 소멸, 자료 연결 등은 이 페이지에서 필수적이다.
	//---------------

	AssetSceneData::AssetSceneData()
	{
		//m_RootNode = new MGRTNodeData;
		//mMetaData = new MGRTMetaData;
	}

	AssetSceneData::~AssetSceneData()
	{
		//delete m_RootNode;
		//delete mMetaData;
	}

	//원하는 Node를 찾는 용도. 
	AssetNodeData* AssetSceneData::FindNode(const std::string& _nodeName)
	{
		return m_NodePlainContainer.at(_nodeName);
	}

	//---------------

	AssetNodeData::AssetNodeData(AssetNodeData* _parentNode)
	{
		//Parent 할당.
		this->m_Parent = _parentNode;
		//그냥 리셋.
		this->m_ParRelativeTransform = DirectX::XMMatrixIdentity();
	}

	AssetNodeData::~AssetNodeData()
	{
		//delete m_Parent;
		//delete m_MetaData;
	}

	//---------------

	AssetBoneData::AssetBoneData()
	{
		//부모 node로, 넣어주기. The bone node in the scene 
		//m_Node = _nodeData;
		//여기서 주의! m_Armature는 Scene에 연동되는 것인가?
		m_Name.clear();
	}

	AssetBoneData::~AssetBoneData()
	{
		//delete m_Armature;
		//delete m_Node;
	}

	//---------------

	AssetSkeletonBoneData::AssetSkeletonBoneData()
	{
		//m_Armature = new MGRTNodeData;
		//m_Node = new MGRTNodeData;
		//mMeshId = new MGRTMeshData;
	}

	AssetSkeletonBoneData::~AssetSkeletonBoneData()
	{
		//delete m_Armature;
		//delete m_Node;
		//delete mMeshId;
	}
}