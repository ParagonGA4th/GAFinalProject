#pragma once

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

#include <dxgi1_2.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <windows.h>

#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using float2 = DirectX::XMFLOAT2;
using float3 = DirectX::XMFLOAT3;
using float4 = DirectX::XMFLOAT4;
using matrix = DirectX::XMMATRIX;

