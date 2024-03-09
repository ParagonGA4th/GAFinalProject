#pragma once
#include <string>
/// <summary>
/// .pganim을 애니메이션 재생에 적합한 형태로 들여오는 Parser.
/// </summary>

namespace Pg::Graphics
{
	class RenderAnimation;
}
namespace Pg::Graphics::Helper
{
	class AnimationParser
	{
	public:
		AnimationParser();
		~AnimationParser();

		void ParseAnimation(const std::string& path, RenderAnimation* anim);
		void Clean();

	private:
		//실제 .pganim 파싱.
		void ParsePgAnim(const std::string& path, RenderAnimation* anim);
		//이미 존재하는 3DModel과 RenderAnimation 관련짓기. (포인터만 받아오기, 개별 다른 Component마다 다른 프레임으로 연산을 해야 하니!)
		void TurnModelToAnimCompatible(const std::string& path, RenderAnimation* anim);
	};
}


