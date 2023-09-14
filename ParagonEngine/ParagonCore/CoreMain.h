

#include "IEngine.h"
#include "IGraphics.h"
#include "TimeManager.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonGameEngine/InputSystem.h"

#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;

/// <summary>
/// 코어
/// 2023. 9. 8. 변지상
/// </summary>
namespace Pg::Core
{
	class CoreMain
	{
	public:
		CoreMain();
		~CoreMain();

		long Initialize(void* hwnd, int screenWidth, int screenHeight);
		void Update();
		void BeginRender();    
		void Render();
		void EndRender();
		void Finalize();

	private:
		const WCHAR* _className;
		const WCHAR* _windowName;

	public:
		void OnWindowResized(int screenWidth, int screenHeight);

	private:
		
		std::unique_ptr<IEngine> _engine;					//게임 엔진
		std::unique_ptr<IGraphics> _graphics;				//그래픽스 엔진
		std::unique_ptr<Pg::Util::Debug::Log> _logger;		//로거
		//Pg::Engine::Input::InputSystem* _inputSystem;

		Time::TimeManager* _timeManager;					//델타타임

	public:
		ID3D11Device* GetGraphicsDevice();
		ID3D11DeviceContext* GetGraphicsDeviceContext();
	};
}

