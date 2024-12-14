#include "msdfgenTest.h"
#include "msdfgen/msdfgen.h"
#include "msdfgen/msdfgen-ext.h"

#ifdef _DEBUG
#pragma comment(lib,"msdfgen-core.lib")
#pragma comment(lib,"msdfgen-ext.lib")

#else
#pragma comment(lib,"msdfgen-core.lib")
#pragma comment(lib,"msdfgen-ext.lib")

#endif // _DEBUG

Pg::Graphics::msdfgenTest::msdfgenTest()
{

	using namespace msdfgen;

	FreetypeHandle* ft = initializeFreetype();
	if (ft) {
		FontHandle* font = loadFont(ft, "C:\\Windows\\Fonts\\Arial.ttf");
		if (font) {
			Shape shape;
			if (loadGlyph(shape, font, 'A')) {
				shape.normalize();
				//                      max. angle
				edgeColoringSimple(shape, 3.0);
				//           image width, height
				Bitmap<float, 3> msdf(32, 32);
				//                     range, scale, translation
				generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));
				savePng(msdf, "output.png");
			}
			destroyFont(font);
		}
		deinitializeFreetype(ft);
	}
}
