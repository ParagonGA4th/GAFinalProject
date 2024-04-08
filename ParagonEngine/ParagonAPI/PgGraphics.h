#pragma once

/// <summary>
/// GraphicsAPI 전반을 제어할 수 있게 열어놓은 API용 클래스.
/// </summary>

namespace Pg::Graphics
{
	class GraphicsApiExporter;
}

namespace Pg::API::Graphics
{
	class PgGraphics
	{
	public:
		PgGraphics();
		~PgGraphics();

		void Initialize();


	private:
		Pg::Graphics::GraphicsApiExporter* _graphicsApiExporter{ nullptr };
	};
}
