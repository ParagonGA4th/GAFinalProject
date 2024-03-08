#pragma once
#include <string>
/// <summary>
/// .pganim을 애니메이션 재생에 적합한 형태로 들여오는 Parser.
/// </summary>

namespace Pg::Graphics
{
	class RenderAnimation;
}
namespace Pg::Graphics
{
	class AnimationParser
	{
	public:
		AnimationParser();
		~AnimationParser();

		void ParseAnimation(const std::string& path, RenderAnimation* anim);
		void Clean();

	private:
		//ParseNodeAnim.
	};
}


