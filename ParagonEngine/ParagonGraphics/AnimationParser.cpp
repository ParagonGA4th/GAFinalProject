#include "AnimationParser.h"
#include "RenderAnimation.h"

#include "../ParagonHelper/pugixml.hpp"

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
		pugi::xml_document doc;
		doc.load_file(path.c_str());

		//.pgmat 파일 형식을 읽어들이기.
		pugi::xml_node tRootNode = doc.child("PgAnim");

		//AnimationName
		{
			pugi::xml_node tNode = tRootNode.child("AnimationName");
			anim->_internalAnimName = tNode.text().get();
		}
		//BasedModelName
		{
			pugi::xml_node tNode = tRootNode.child("BasedModelName");
			anim->_basedModelName = tNode.text().get();
		}
		//DurationTick
		{
			pugi::xml_node tNode = tRootNode.child("DurationTick");
			anim->_durationTick = std::stof(tNode.text().get());
		}
		//TicksPerSecond
		{
			pugi::xml_node tNode = tRootNode.child("TicksPerSecond");
			anim->_ticksPerSecond = std::stof(tNode.text().get());
		}
		//NumChannels
		{
			pugi::xml_node tNode = tRootNode.child("NumChannels");
			anim->_numChannels = std::stoul(tNode.text().get());
		}
		//이제 개별적인 NodeAnimList 내부에서 개별적인 NodeAnim을 파싱하는 함수를 만들고 /
		//어떻게 받은 pganim을 가공 / 저장할지를 적어야 한다.
		///TODO: 240308

	}

	void AnimationParser::Clean()
	{

	}

}
