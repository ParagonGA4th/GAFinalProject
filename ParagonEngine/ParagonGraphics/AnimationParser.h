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
		//실제 .pganim 파싱.
		void ParsePgAnim(const std::string& path, RenderAnimation* anim);
		//이미 존재하는 3DModel과 RenderAnimation 관련짓기. (포인터만 받아오기, 개별 다른 Component마다 다른 프레임으로 연산을 해야 하니!)
		void TurnModelToAnimCompatible(const std::string& path, RenderAnimation* anim);
		//RenderAnimation에서 Node 내부를 Hierarchy처럼 내려오면서 재귀적으로 업데이트할 NodeAnim 

	};
}


