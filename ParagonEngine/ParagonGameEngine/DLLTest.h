#pragma once

#ifdef CREATEDLL_EXPORTS
#define TEST_DECLSPEC __declspec(dllexport)
#else
#define TEST_DECLSPEC __declspec(dllimport)
#endif 

extern "C" TEST_DECLSPEC double Sum(double a, double b);

namespace Pg::Graphics
{

} 

