#pragma once
#include "../ParagonData/VisualEffectData.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include "../ParagonUtil/Tween.h"
#include "VisualEffectGraphicsSet.h"
#include "SystemVertexShader.h"

#include <vector>
#include <memory>
#include <tuple>
#include <unordered_map>

#include <dxtk/CommonStates.h>

namespace Pg::Graphics
{
	class LowDX11Storage;

	namespace Manager
	{
		class GraphicsResourceManager;
	}
}

/// <summary>
/// Mesh���õ� ����Ʈ�� �Ϸ���,
/// Renderer�� ������ Script�� �����ִ� �������� �ؾ� �Ѵ�.
/// ����� BillBoard 2D / 3D�� �Ҵ��ϰ� ����.
/// </summary>

namespace Pg::Util
{
	namespace Time
	{
		class TimeSystem;
	}
}

namespace Pg::Graphics
{
	struct VERenderingSet
	{
		VERenderingSet(Pg::Data::VisualEffectData veData) :
			_visualEffectData(veData)
		{
			_veGraphicsSet = new VisualEffectGraphicsSet();
		}
		~VERenderingSet()
		{
			if (_veGraphicsSet != nullptr) { delete _veGraphicsSet; }
		}
		Pg::Data::VisualEffectData _visualEffectData;

		VisualEffectGraphicsSet* _veGraphicsSet{ nullptr };
		//�� ������ ��Ÿ ������ �ʿ��ϴٸ� ���Ե� ��.
	};
}

namespace Pg::Graphics
{
	struct D3DCarrier;

	class VisualEffectRenderer
	{
		//Billboard Render Pass�� ���� �� �ִ�.
		friend class VisualEffectRenderPass;
	public:
		VisualEffectRenderer();
		~VisualEffectRenderer();

	public:
		//CSV���� �ε�� ����ü����, �ε�. 
		void LoadAllEffects(const std::vector<Pg::Data::VisualEffectData>& vec);

		//Visual Effect Object �� �� ����.
		void ResetVisualEffectObjects();

		//Visual Effect Render Object�� �ٸ� ������Ʈ�ʹ� �޸�,
		//����ü�� ����, �̸� Rendering Engine�� ����ϴ� ������� �Ǿ� �ִ�.
		//Tween�� ���� �����ϰ� �ϱ� ���� �ٸ�, ���� �����ؾ� �Ѵ�.
		//������ Script�� �����ȴ�. Tween �� ������ ���� ��, Null Reference�� ���� ���ؼ�.
		//��������� ������ ���̰� ������, Remove�� ȣ���ؾ� �Ѵ�.
		//��� ���� Ű�� ������, SetActive�� Ȱ���ϸ� �ȴ�.
		void RegisterVisualEffectObject(const std::string& effectName, Pg::Data::VisualEffectRenderObject* vfxObj);
		void RemoveVisualEffectObject(Pg::Data::VisualEffectRenderObject* vfxObj);

		//Main Render Loop���� ���ư��� ȿ���̴�.
		void Render(const D3DCarrier* carrier, Pg::Data::CameraData* camData);

		//�ε����� ������ �� �־�� �ϱ⿡, ����Ʈ ��ȿ + ManualSwitchMode�� ���� ������ nullptr ��ȯ.
		unsigned int* GetEffectTextureIndexPointer(const std::string& effectName);
	private:
		void LoadSingleEffect(VERenderingSet* veSet);
		//void Load3DSpaceEffect(VERenderingSet* veSet);
		//void Render3dSpaceQuadEffect(VERenderingSet* veSet, Pg::Data::VisualEffectRenderObject* veObj);
		//void Render2dSpaceQuadEffect(VERenderingSet* veSet, Pg::Data::VisualEffectRenderObject* veObj);
		//�߰��� �� �߰��� �� ����.
	private:
		Manager::GraphicsResourceManager* _graphicsResourceManager{ nullptr };
		LowDX11Storage* _DXStorage{ nullptr };
		
		//���ҽ� �����. EffectName // Data.
		std::unordered_map<std::string, std::unique_ptr<VERenderingSet>> _visualEffectsMap;
		//����Ʈ�� ��û ���� �͵� �ƴϰ�, �׳� 1 ����Ʈ�� Rendering State �ٲ��شٰ� ��������.

		//���� �����Ǵ� ������Ʈ ���.
		std::unordered_map<VERenderingSet*, std::vector<Pg::Data::VisualEffectRenderObject*>> _currentRenderingMap;
		
		//CommonState ����.
		std::unique_ptr<DirectX::CommonStates> _commonStates{ nullptr };
	
	private:
		Pg::Util::Time::TimeSystem* _timeSystem{ nullptr };
	};
}
