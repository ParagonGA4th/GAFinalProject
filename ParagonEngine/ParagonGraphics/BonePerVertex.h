#pragma once
namespace Pg::Graphics
{
	class BonePerVertex
	{
	public:
		BonePerVertex();

		void AddBoneData(unsigned int BoneID, float Weight);
		bool isInit = false;

		//이걸 보고 Vertex Array를 반영할지 안할지를 보면 된다.
		int idWeightSize = 0;

		unsigned int IDs[4]; //!< An array of 4 bone Ids that influence a single vertex.
		float Weights[4]; //!< An array of the weight influence per bone. 
	};
}
