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

		//.pgmat 파일 형식을 읽어들이기.
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

		//이제 개별적인 NodeAnimList 내부에서 개별적인 NodeAnim을 파싱하는 함수를 만들고 /
		//어떻게 받은 pganim을 가공 / 저장할지를 적어야 한다.
		//시간별로 영향을 줄 노드를 고정?
		//아니면 기존 방식대로 매번 파싱하는 코드?

		//NodeAnimList
		{
			pugi::xml_node tThisParentNode = tRootNode.child("NodeAnimList");

			//내부 
			for (pugi::xml_node tNodeAnimNode = tThisParentNode.first_child();
				tNodeAnimNode; tNodeAnimNode = tNodeAnimNode.next_sibling())
			{
				//NodeAnim Layer.
				auto tNewNodeAnim = std::make_unique<NodeAnim_AssetData>();

				//NodeAnim 정보 조작.
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
						//개별적인 VectorKey 파싱.
						for (pugi::xml_node tKeyNode = tListNode.first_child();
							tKeyNode; tKeyNode = tKeyNode.next_sibling())
						{
							pugi::xml_node tTimeNode = tKeyNode.child("Time");
							tNewNodeAnim->_positionKeyList.at(tInternalCount)._time = tTimeNode.text().as_float();

							pugi::xml_node tValueNode = tKeyNode.child("Value");
							std::string tValueString = tValueNode.text().get();

							std::istringstream tStream(tValueString);
							std::string tStrBuf;

							//직접적으로 집어넣기.
							//assert(std::getline(tStream, tStrBuf, ','));
							std::getline(tStream, tStrBuf, ',');
							tNewNodeAnim->_positionKeyList.at(tInternalCount)._value.x = std::stof(tStrBuf);
							//assert(std::getline(tStream, tStrBuf, ','));
							std::getline(tStream, tStrBuf, ',');
							tNewNodeAnim->_positionKeyList.at(tInternalCount)._value.y = std::stof(tStrBuf);
							//assert(std::getline(tStream, tStrBuf, ','));
							std::getline(tStream, tStrBuf, ',');
							tNewNodeAnim->_positionKeyList.at(tInternalCount)._value.z = std::stof(tStrBuf);

							//다음 Index로.
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
						//개별적인 QuatKey 파싱.
						for (pugi::xml_node tKeyNode = tListNode.first_child();
							tKeyNode; tKeyNode = tKeyNode.next_sibling())
						{
							pugi::xml_node tTimeNode = tKeyNode.child("Time");
							tNewNodeAnim->_rotationKeyList.at(tInternalCount)._time = tTimeNode.text().as_float();

							pugi::xml_node tValueNode = tKeyNode.child("Value");
							std::string tValueString = tValueNode.text().get();

							std::istringstream tStream(tValueString);
							std::string tStrBuf;

							//직접적으로 집어넣기.
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

							//다음 Index로.
							tInternalCount++;
						}
					}
				}
				{
					//여기는 Scaling: 만약 Scaling이 있으면 반영, 없으면 무시하게 해야 함.
					pugi::xml_node tNode = tNodeAnimNode.child("NumScalingKeys");
					if (tNode)
					{
						//NumScaling이 있으면 -> Scaling 관련 정보가 전부 다 있다는 뜻.
						tNewNodeAnim->_numScalingKeys = tNode.text().as_uint();

						if (tNewNodeAnim->_numScalingKeys > 0)
						{
							pugi::xml_node tListNode = tNodeAnimNode.child("ScalingKeyList");
							tNewNodeAnim->_scalingKeyList.resize(tNewNodeAnim->_numScalingKeys);

							unsigned int tInternalCount = 0;
							//개별적인 VectorKey 파싱.
							for (pugi::xml_node tKeyNode = tListNode.first_child();
								tKeyNode; tKeyNode = tKeyNode.next_sibling())
							{
								pugi::xml_node tTimeNode = tKeyNode.child("Time");
								tNewNodeAnim->_scalingKeyList.at(tInternalCount)._time = tTimeNode.text().as_float();

								pugi::xml_node tValueNode = tKeyNode.child("Value");
								std::string tValueString = tValueNode.text().get();

								std::istringstream tStream(tValueString);
								std::string tStrBuf;

								//직접적으로 집어넣기.
								//assert(std::getline(tStream, tStrBuf, ','));
								std::getline(tStream, tStrBuf, ',');
								tNewNodeAnim->_scalingKeyList.at(tInternalCount)._value.x = std::stof(tStrBuf);
								//assert(std::getline(tStream, tStrBuf, ','));
								std::getline(tStream, tStrBuf, ',');
								tNewNodeAnim->_scalingKeyList.at(tInternalCount)._value.y = std::stof(tStrBuf);
								//assert(std::getline(tStream, tStrBuf, ','));
								std::getline(tStream, tStrBuf, ',');
								tNewNodeAnim->_scalingKeyList.at(tInternalCount)._value.z = std::stof(tStrBuf);

								//다음 Index로.
								tInternalCount++;
							}
						}
					}
					else
					{
						//없으면 기록을 하지 않는다.
						tNewNodeAnim->_numScalingKeys = 0;
					}
				}
				//unique_ptr은 복사 불가. move로 투입.
				anim->_animAssetData->_channelList.push_back(std::move(tNewNodeAnim));
			}
		}
	}

	void AnimationParser::TurnModelToAnimCompatible(const std::string& path, RenderAnimation* anim)
	{
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//RenderAnimation이라는 리소스 자료 자체는 1개여야 하지만, 이로 인해 재생되는 애니메이션은 한 씬 안에서 1개-N개가 될 수 있다.
		//SkinnedMeshRenderObject등 RenderObject에서 개별적으로 직접적으로 GPU에 대입할 행렬 목록을 가지고 있어야 하고,
		//이를 위한 기반 연산 정보를 Animation 딴에서 가지고 있는 것!
		{
			//.fbx가 없을 경우에 (string) 추가해주는 hotfix.
			const std::string suffix = ".fbx";
			const size_t suffixLen = suffix.length();
			std::string input = anim->_animAssetData->_basedModelName;
			// 접미어가 string의 마지막 부분에 있는지 확인..
			if (input.length() < suffixLen || input.substr(input.length() - suffixLen) != suffix)
			{
				input += suffix;
			}
			anim->_animAssetData->_basedModelName = input;
		}

		const std::string& tBasedModelName = anim->_animAssetData->_basedModelName;
		CustomAssert(GraphicsResourceManager::Instance()->IsExistResourceByName(tBasedModelName) &&
			"무조건 이 단계에서 자료가 존재해야 한다! 아니면 Animation이 기반으로 할 3D모델이 로드 안되었다는 말");

		Asset3DModelData* tBasedModelPtr = static_cast<Asset3DModelData*>(GraphicsResourceManager::Instance()->GetResourceByName(
			tBasedModelName, Pg::Data::Enums::eAssetDefine::_3DMODEL).get());
		CustomAssert(tBasedModelPtr->IsSkinned() && "무조건 Skinned 모델이어야 Animation이 호환된다.");

		//기존의 Linearize하지 않기로 함.
		//바로 값 투입!

		//Path에서 Animation을 호출할 때 활용될 이름을 Path로부터 추출, -> stem 투입.
		//이제, 자기 자신이 호환된다는 것을 AssetSkinnedData에게 알려주자.
		//Ex. test_run.pganim이라면, test_run이 이름으로 등록될 것.
		std::filesystem::path tPath(path);
		tBasedModelPtr->_assetSkinnedData->_viableAnimations.insert(std::make_pair(tPath.filename().generic_string(), anim));
	}

}
