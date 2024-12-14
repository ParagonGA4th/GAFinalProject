
#include "IEngine.h"
#include "IGraphics.h"
#include "IUtil.h"
#include "IAPI.h"
#include "../ParagonUtil/Log.h"

//#include "../ParagonGameEngine/InputSystem.h"
#include <d3d11.h>

#include <vector>
#include <memory>

//struct ID3D11Device;
//struct ID3D11DeviceContext;

/// <summary>
/// �ھ�
/// 2023. 9. 8. ������
/// 
/// ->
/// </summary>

namespace Pg::Data
{
	class GameObject;
}

namespace Pg::Core
{
	class EngineGraphicsAdapter;
	class EditorAdapter;

	namespace Manager
	{
		class AssetManager;
	}
}

namespace Pg::Core
{
	class ProcessMain
	{
		friend class Pg::Core::Manager::AssetManager;
		friend class Pg::Core::EditorAdapter;
		friend class Pg::Core::EngineGraphicsAdapter;
	public:
		ProcessMain();
		~ProcessMain();

		long Initialize(void* hwnd, int screenWidth, int screenHeight);
		void Update();
		void BeginRender();    
		void Render();
		//Pg::Data::GameObject* PassPickedObject();
		void SetOutlinedObject(Pg::Data::GameObject* obj);
		void FinalRender();
		void EndRender();
		void Finalize();

		//.pgproject�� �ε�Ǿ��� ��, �� �ѹ� ����.
		void OnParagonProjectLoad(const std::vector<Pg::Data::Scene*>& sceneVec);

	private:
		const WCHAR* _className;
		const WCHAR* _windowName;

	public:
		void OnWindowResized(int screenWidth, int screenHeight);
		ID3D11Device* GetGraphicsDevice();
		ID3D11DeviceContext* GetGraphicsDeviceContext();

		//Engine & Graphics <-> CoreMain �����ϴ� ����� ����.
		EngineGraphicsAdapter* GetEngineGraphicsAdapter();

		//Editor <-> CoreMain �����ϴ� ����� ����.
		EditorAdapter* GetEditorAdapter();

		std::vector<std::string> GetAssetList(Pg::Data::Enums::eAssetDefine define);
	private:
		std::unique_ptr<IUtil> _util;
		std::unique_ptr<IAPI> _api;					
		Manager::AssetManager* _assetManager;				//��¸Ŵ���

	private:
		std::unique_ptr<EngineGraphicsAdapter> _engineGraphicsAdapter;
		std::unique_ptr<EditorAdapter> _editorAdapter;
	};
}

