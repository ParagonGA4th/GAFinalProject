#pragma once
#include <string>
/// <summary>
/// .pganim�� �ִϸ��̼� ����� ������ ���·� �鿩���� Parser.
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
		//���� .pganim �Ľ�.
		void ParsePgAnim(const std::string& path, RenderAnimation* anim);
		//�̹� �����ϴ� 3DModel�� RenderAnimation ��������. (�����͸� �޾ƿ���, ���� �ٸ� Component���� �ٸ� ���������� ������ �ؾ� �ϴ�!)
		void TurnModelToAnimCompatible(const std::string& path, RenderAnimation* anim);
		//RenderAnimation���� Node ���θ� Hierarchyó�� �������鼭 ��������� ������Ʈ�� NodeAnim 

	};
}


