#include "AnimationParser.h"
#include "RenderAnimation.h"
#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"
#include "Asset3DModelData.h"
#include "AssetModelDataDefine.h"

#include "../ParagonHelper/pugixml.hpp"
#include "../ParagonUtil/CustomAssert.h"

#include <utility>
#include <filesystem>
#include <sstream>
#include <cassert>

namespace Pg::Graphics
{
	AnimationParser::AnimationParser()
	{

	}

	AnimationParser::~AnimationParser()
	{

	}

	void AnimationParser::ParseAnimation(const std::string& path, RenderAnimation* anim)
	{
		ParsePgAnim(path, anim);
		TurnModelToAnimCompatible(path, anim);
	}

	void AnimationParser::Clean()
	{

	}

	void AnimationParser::ParsePgAnim(const std::string& path, RenderAnimation* anim)
	{
		pugi::xml_document doc;
		doc.load_file(path.c_str());

		//.pgmat ���� ������ �о���̱�.
		pugi::xml_node tRootNode = doc.child("PgAnim");

		//AnimationName
		{
			pugi::xml_node tNode = tRootNode.child("AnimationName");
			anim->_animAssetData->_animName = tNode.text().get();
		}
		//BasedModelName
		{
			pugi::xml_node tNode = tRootNode.child("BasedModelName");
			anim->_animAssetData->_basedModelName = tNode.text().get();
		}
		//DurationTick
		{
			pugi::xml_node tNode = tRootNode.child("DurationTick");
			anim->_animAssetData->_durationTick = std::stof(tNode.text().get());
		}
		//TicksPerSecond
		{
			pugi::xml_node tNode = tRootNode.child("TicksPerSecond");
			anim->_animAssetData->_ticksPerSecond = std::stof(tNode.text().get());
		}
		//NumChannels
		{
			pugi::xml_node tNode = tRootNode.child("NumChannels");
			anim->_animAssetData->_numChannels = std::stoul(tNode.text().get());
		}

		//���� �������� NodeAnimList ���ο��� �������� NodeAnim�� �Ľ��ϴ� �Լ��� ����� /
		//��� ���� pganim�� ���� / ���������� ����� �Ѵ�.
		//�ð����� ������ �� ��带 ����?
		//�ƴϸ� ���� ��Ĵ�� �Ź� �Ľ��ϴ� �ڵ�?

		//NodeAnimList
		{
			pugi::xml_node tThisParentNode = tRootNode.child("NodeAnimList");

			//���� 
			for (pugi::xml_node tNodeAnimNode = tThisParentNode.first_child();
				tNodeAnimNode; tNodeAnimNode = tNodeAnimNode.next_sibling())
			{
				//NodeAnim Layer.
				auto tNewNodeAnim = std::make_unique<NodeAnim_AssetData>();

				//NodeAnim ���� ����.
				{
					pugi::xml_node tNode = tNodeAnimNode.child("NodeName");
					tNewNodeAnim->_nodeName = tNode.text().get();
				}
				{
					//NumPositionKeys
					pugi::xml_node tNode = tNodeAnimNode.child("NumPositionKeys");
					tNewNodeAnim->_numPositionKeys = tNode.text().as_uint();

					if (tNewNodeAnim->_numPositionKeys > 0)
					{
						pugi::xml_node tListNode = tNodeAnimNode.child("PositionKeyList");
						tNewNodeAnim->_positionKeyList.resize(tNewNodeAnim->_numPositionKeys);

						unsigned int tInternalCount = 0;
						//�������� VectorKey �Ľ�.
						for (pugi::xml_node tKeyNode = tListNode.first_child();
							tKeyNode; tKeyNode = tKeyNode.next_sibling())
						{
							pugi::xml_node tTimeNode = tKeyNode.child("Time");
							tNewNodeAnim->_positionKeyList.at(tInternalCount)._time = tTimeNode.text().as_float();

							pugi::xml_node tValueNode = tKeyNode.child("Value");
							std::string tValueString = tValueNode.text().get();

							std::istringstream tStream(tValueString);
							std::string tStrBuf;

							//���������� ����ֱ�.
							//assert(std::getline(tStream, tStrBuf, ','));
							std::getline(tStream, tStrBuf, ',');
							tNewNodeAnim->_positionKeyList.at(tInternalCount)._value.x = std::stof(tStrBuf);
							//assert(std::getline(tStream, tStrBuf, ','));
							std::getline(tStream, tStrBuf, ',');
							tNewNodeAnim->_positionKeyList.at(tInternalCount)._value.y = std::stof(tStrBuf);
							//assert(std::getline(tStream, tStrBuf, ','));
							std::getline(tStream, tStrBuf, ',');
							tNewNodeAnim->_positionKeyList.at(tInternalCount)._value.z = std::stof(tStrBuf);

							//���� Index��.
							tInternalCount++;
						}
					}
				}
				{
					//NumRotationKeys
					pugi::xml_node tNode = tNodeAnimNode.child("NumRotationKeys");
					tNewNodeAnim->_numRotationKeys = tNode.text().as_uint();

					if (tNewNodeAnim->_numRotationKeys > 0)
					{
						pugi::xml_node tListNode = tNodeAnimNode.child("RotationKeyList");
						tNewNodeAnim->_rotationKeyList.resize(tNewNodeAnim->_numRotationKeys);

						unsigned int tInternalCount = 0;
						//�������� QuatKey �Ľ�.
						for (pugi::xml_node tKeyNode = tListNode.first_child();
							tKeyNode; tKeyNode = tKeyNode.next_sibling())
						{
							pugi::xml_node tTimeNode = tKeyNode.child("Time");
							tNewNodeAnim->_rotationKeyList.at(tInternalCount)._time = tTimeNode.text().as_float();

							pugi::xml_node tValueNode = tKeyNode.child("Value");
							std::string tValueString = tValueNode.text().get();

							std::istringstream tStream(tValueString);
							std::string tStrBuf;

							//���������� ����ֱ�.
							//assert(std::getline(tStream, tStrBuf, ','));
							std::getline(tStream, tStrBuf, ',');
							tNewNodeAnim->_rotationKeyList.at(tInternalCount)._value.x = std::stof(tStrBuf);
							//assert(std::getline(tStream, tStrBuf, ','));
							std::getline(tStream, tStrBuf, ',');
							tNewNodeAnim->_rotationKeyList.at(tInternalCount)._value.y = std::stof(tStrBuf);
							//assert(std::getline(tStream, tStrBuf, ','));
							std::getline(tStream, tStrBuf, ',');
							tNewNodeAnim->_rotationKeyList.at(tInternalCount)._value.z = std::stof(tStrBuf);
							//assert(std::getline(tStream, tStrBuf, ','));
							std::getline(tStream, tStrBuf, ',');
							tNewNodeAnim->_rotationKeyList.at(tInternalCount)._value.w = std::stof(tStrBuf);

							//���� Index��.
							tInternalCount++;
						}
					}
				}
				{
					//����� Scaling: ���� Scaling�� ������ �ݿ�, ������ �����ϰ� �ؾ� ��.
					pugi::xml_node tNode = tNodeAnimNode.child("NumScalingKeys");
					if (tNode)
					{
						//NumScaling�� ������ -> Scaling ���� ������ ���� �� �ִٴ� ��.
						tNewNodeAnim->_numScalingKeys = tNode.text().as_uint();

						if (tNewNodeAnim->_numScalingKeys > 0)
						{
							pugi::xml_node tListNode = tNodeAnimNode.child("ScalingKeyList");
							tNewNodeAnim->_scalingKeyList.resize(tNewNodeAnim->_numScalingKeys);

							unsigned int tInternalCount = 0;
							//�������� VectorKey �Ľ�.
							for (pugi::xml_node tKeyNode = tListNode.first_child();
								tKeyNode; tKeyNode = tKeyNode.next_sibling())
							{
								pugi::xml_node tTimeNode = tKeyNode.child("Time");
								tNewNodeAnim->_scalingKeyList.at(tInternalCount)._time = tTimeNode.text().as_float();

								pugi::xml_node tValueNode = tKeyNode.child("Value");
								std::string tValueString = tValueNode.text().get();

								std::istringstream tStream(tValueString);
								std::string tStrBuf;

								//���������� ����ֱ�.
								//assert(std::getline(tStream, tStrBuf, ','));
								std::getline(tStream, tStrBuf, ',');
								tNewNodeAnim->_scalingKeyList.at(tInternalCount)._value.x = std::stof(tStrBuf);
								//assert(std::getline(tStream, tStrBuf, ','));
								std::getline(tStream, tStrBuf, ',');
								tNewNodeAnim->_scalingKeyList.at(tInternalCount)._value.y = std::stof(tStrBuf);
								//assert(std::getline(tStream, tStrBuf, ','));
								std::getline(tStream, tStrBuf, ',');
								tNewNodeAnim->_scalingKeyList.at(tInternalCount)._value.z = std::stof(tStrBuf);

								//���� Index��.
								tInternalCount++;
							}
						}
					}
					else
					{
						//������ ����� ���� �ʴ´�.
						tNewNodeAnim->_numScalingKeys = 0;
					}
				}
				//unique_ptr�� ���� �Ұ�. move�� ����.
				anim->_animAssetData->_channelList.push_back(std::move(tNewNodeAnim));
			}
		}
	}

	void AnimationParser::TurnModelToAnimCompatible(const std::string& path, RenderAnimation* anim)
	{
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//RenderAnimation�̶�� ���ҽ� �ڷ� ��ü�� 1������ ������, �̷� ���� ����Ǵ� �ִϸ��̼��� �� �� �ȿ��� 1��-N���� �� �� �ִ�.
		//SkinnedMeshRenderObject�� RenderObject���� ���������� ���������� GPU�� ������ ��� ����� ������ �־�� �ϰ�,
		//�̸� ���� ��� ���� ������ Animation ������ ������ �ִ� ��!
		{
			//.fbx�� ���� ��쿡 (string) �߰����ִ� hotfix.
			const std::string suffix = ".fbx";
			const size_t suffixLen = suffix.length();
			std::string input = anim->_animAssetData->_basedModelName;
			// ���̾ string�� ������ �κп� �ִ��� Ȯ��..
			if (input.length() < suffixLen || input.substr(input.length() - suffixLen) != suffix)
			{
				input += suffix;
			}
			anim->_animAssetData->_basedModelName = input;
		}

		const std::string& tBasedModelName = anim->_animAssetData->_basedModelName;
		CustomAssert(GraphicsResourceManager::Instance()->IsExistResourceByName(tBasedModelName) &&
			"������ �� �ܰ迡�� �ڷᰡ �����ؾ� �Ѵ�! �ƴϸ� Animation�� ������� �� 3D���� �ε� �ȵǾ��ٴ� ��");

		Asset3DModelData* tBasedModelPtr = static_cast<Asset3DModelData*>(GraphicsResourceManager::Instance()->GetResourceByName(
			tBasedModelName, Pg::Data::Enums::eAssetDefine::_3DMODEL).get());
		CustomAssert(tBasedModelPtr->IsSkinned() && "������ Skinned ���̾�� Animation�� ȣȯ�ȴ�.");

		//������ Linearize���� �ʱ�� ��.
		//�ٷ� �� ����!

		//Path���� Animation�� ȣ���� �� Ȱ��� �̸��� Path�κ��� ����, -> stem ����.
		//����, �ڱ� �ڽ��� ȣȯ�ȴٴ� ���� AssetSkinnedData���� �˷�����.
		//Ex. test_run.pganim�̶��, test_run�� �̸����� ��ϵ� ��.
		std::filesystem::path tPath(path);
		tBasedModelPtr->_assetSkinnedData->_viableAnimations.insert(std::make_pair(tPath.filename().generic_string(), anim));
	}

}
