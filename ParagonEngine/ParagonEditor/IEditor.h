#pragma once

namespace Pg::Editor
{
	class IEditor
	{
	public:
		virtual void Initialize(void* hWnd, float width, float height) abstract;
		virtual void Update() abstract;
		virtual void Finalize() abstract;
	};
}