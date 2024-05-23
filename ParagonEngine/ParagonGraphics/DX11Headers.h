#pragma once

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

#include "../ParagonMath/PgMath.h"
#include "../ParagonUtil/Log.h"
#include <dxgi1_2.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <windows.h>
#include <cassert>

#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using float2 = DirectX::XMFLOAT2;
using float3 = DirectX::XMFLOAT3;
using float4 = DirectX::XMFLOAT4;
using float4x4 = DirectX::XMMATRIX;

//DXTK의 ThrowIfFailed 매크로 함수 추가!
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)									\
	{									\
		HRESULT hr = (x);						\
		if(FAILED(hr))							\
		{								\
			assert(false); 		\
			LPWSTR output;                                    	\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |		\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,			\
				NULL,						\
				hr,						\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_ENGLISH_US),	\
				(LPTSTR) &output,				\
				0,						\
				NULL);					        \
			MessageBox(NULL, output, L"Error", MB_OK);		\
		}								\
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

