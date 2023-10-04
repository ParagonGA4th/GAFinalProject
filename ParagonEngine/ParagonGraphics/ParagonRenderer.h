#pragma once

#include "../ParagonCore/GameObject.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonCore.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonCore.lib")
#endif // _DEBUG

/// <summary>
/// Paragon Renderer
/// 
/// 2023. 09. 12.
/// </summary>
/// 
/// 
namespace Pg::Graphics
{
	class LowDX11Logic;
	class LowDX11Storage;

	class ParagonRenderer
	{
	public:
		ParagonRenderer();
		~ParagonRenderer();

		void BeginRender();
		void Render(Pg::Core::GameObject* gameObject);
		void EndRender();



	private:
		LowDX11Storage* _DXStorage = nullptr;
		LowDX11Logic* _DXLogic = nullptr;
	};

}

