#pragma once

/// <summary>
/// Editor�� Mode�� ���� ��� �Ǵ��� �����ϴ� Enum.
/// </summary>

namespace Pg::Data::Enums
{
	enum class eEditorMode
	{
		_NONE, // �ʱⰪ
		_EDIT, // ���� ����, ������ ���� ������ ����.
		_PLAY, // ����ϴ� ����.
		_GAME, // ���� ������ ����Ǵ� ����, EDITOR GUI �� ����.
	};
}