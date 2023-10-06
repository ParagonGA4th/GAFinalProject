#pragma once

#include "../ParagonData/GameObject.h"


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
		void Render(Pg::Data::GameObject* gameObject);
		void EndRender();



	private:
		LowDX11Storage* _DXStorage = nullptr;
		LowDX11Logic* _DXLogic = nullptr;
	};

}

