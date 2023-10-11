#pragma once

namespace Pg::Editor
{
	class IEditor
	{
	public:
		virtual void Initialize() abstract;
		virtual void Run() abstract;
		virtual void Finalize() abstract;
	};
}