#pragma once


#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonGraphics.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonGraphics.lib")
#endif // _DEBUG

namespace Pg::Core
{
	class CoreMain
	{
	public:
		CoreMain();

		void Initialize();
		void Finalize();
	};
}

