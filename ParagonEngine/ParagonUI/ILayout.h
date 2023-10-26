#pragma once

namespace Pg::UI
{
	class ILayout
	{
		friend class Panel;
	public:
		virtual void CreateWidget() abstract;
	private:
		virtual void Update() abstract;
	};
}


