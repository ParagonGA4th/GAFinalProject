//#include "TestCube.h"
//
//#include "DX11Headers.h"
//#include "MathHelper.h"
//#include "LowDX11Storage.h"
//#include "LayoutDefine.h"
//
//#include "../ParagonData/Transform.h"
//#include "../ParagonData/CameraData.h"
//
//Pg::Graphics::TestCube::TestCube()
//	: RenderableObject(),
//	_cbData()
//{
//
//}
//
//void Pg::Graphics::TestCube::Draw(Pg::Data::Transform& transform, Pg::Data::CameraData& camData)
//{
//	// 행렬 셋팅
//	DirectX::XMFLOAT4X4 tWorldTM = Helper::MathHelper::PG2XM_FLOAT4X4(transform.GetWorldTM());
//	DirectX::XMMATRIX tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);
//
//	DirectX::XMFLOAT4X4 tViewTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData._viewMatrix);
//	DirectX::XMMATRIX tViewTMMat = DirectX::XMLoadFloat4x4(&tViewTM);
//
//	DirectX::XMFLOAT4X4 tProjTM = Helper::MathHelper::PG2XM_FLOAT4X4(camData._projMatrix);
//	DirectX::XMMATRIX tProjTMMat = DirectX::XMLoadFloat4x4(&tProjTM);
//
//	DirectX::XMFLOAT3 tCameraPosition = Helper::MathHelper::PG2XM_FLOAT3(camData._position);
//	DirectX::XMVECTOR tCameraPositionVec = DirectX::XMLoadFloat3(&tCameraPosition);
//	DirectX::XMMATRIX tCameraPositionMat = DirectX::XMMatrixTranslationFromVector(tCameraPositionVec);
//
//	float tCamDistance = 0.0f;
//	DirectX::XMStoreFloat(&tCamDistance, DirectX::XMVector3Length(tCameraPositionVec));
//
//	_cbData.gCBuf_World = tWorldTMMat;
//	_cbData.gCBuf_WorldViewProj = DirectX::XMMatrixMultiply(tWorldTMMat, DirectX::XMMatrixMultiply(tViewTMMat, tProjTMMat));
//
//	// 상수버퍼 업데이트
//	for (auto& cb : _vertexShader->_constantBuffers)
//	{
//		cb->UpdateAndBind();
//	}
//
//	// IA 바인딩
//	_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	BindBuffers();
//
//	// 그리기
//	_DXStorage->_deviceContext->DrawIndexed(36, 0, 0);
//}
//
//void Pg::Graphics::TestCube::Draw()
//{
//
//}
//
//void Pg::Graphics::TestCube::BuildBuffers()
//{
//	std::vector<LayoutDefine::Vin1stStatic> VBData;
//	std::vector<int> IBData;
//
//	VBData.emplace_back(LayoutDefine::Vin1stStatic
//		{
//		DirectX::XMFLOAT3 {-1.0f, 1.0f, -1.0f},			// PosL
//		DirectX::XMFLOAT3 { },							// NormalL
//		DirectX::XMFLOAT3 { },							// TangentL
//		DirectX::XMFLOAT4 {1.0f, 0.0f, 0.0f, 1.0f},		// Color
//		DirectX::XMFLOAT3 { },							// Tex
//		0												// MatID
//		});
//
//	VBData.emplace_back(LayoutDefine::Vin1stStatic
//		{
//		DirectX::XMFLOAT3 {1.0f, 1.0f, -1.0f},
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT4 {1.0f, 0.0f, 0.0f, 1.0f},
//		DirectX::XMFLOAT3 { },
//		0
//		});
//
//	VBData.emplace_back(LayoutDefine::Vin1stStatic
//		{
//		DirectX::XMFLOAT3 {-1.0f, 1.0f, 1.0f},
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT4 {1.0f, 0.0f, 0.0f, 1.0f},
//		DirectX::XMFLOAT3 { },
//		0
//		});
//
//	VBData.emplace_back(LayoutDefine::Vin1stStatic
//		{
//		DirectX::XMFLOAT3 {1.0f, 1.0f, 1.0f},
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT4 {1.0f, 0.0f, 0.0f, 1.0f},
//		DirectX::XMFLOAT3 { },
//		0
//		});
//
//	VBData.emplace_back(LayoutDefine::Vin1stStatic
//		{
//		DirectX::XMFLOAT3 {1.0f, -1.0f, -1.0f},
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT4 {1.0f, 0.0f, 0.0f, 1.0f},
//		DirectX::XMFLOAT3 { },
//		0
//		});
//
//	VBData.emplace_back(LayoutDefine::Vin1stStatic
//		{
//		DirectX::XMFLOAT3 {-1.0f, -1.0f, -1.0f},
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT4 {0.0f, 1.0f, 0.0f, 1.0f},
//		DirectX::XMFLOAT3 { },
//		0
//		});
//
//	VBData.emplace_back(LayoutDefine::Vin1stStatic
//		{
//		DirectX::XMFLOAT3 {-1.0f, -1.0f, 1.0f},
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT4 {1.0f, 0.0f, 0.0f, 1.0f},
//		DirectX::XMFLOAT3 { },
//		0
//		});
//
//	VBData.emplace_back(LayoutDefine::Vin1stStatic
//		{
//		DirectX::XMFLOAT3 {1.0f, -1.0f, 1.0f},
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT3 { },
//		DirectX::XMFLOAT4 {1.0f, 0.0f, 0.0f, 1.0f},
//		DirectX::XMFLOAT3 { },
//		0
//		});
//
//	IBData.emplace_back(0);
//	IBData.emplace_back(2);
//	IBData.emplace_back(1);
//
//	IBData.emplace_back(2);
//	IBData.emplace_back(3);
//	IBData.emplace_back(1);
//
//	IBData.emplace_back(7);
//	IBData.emplace_back(3);
//	IBData.emplace_back(2);
//
//	IBData.emplace_back(2);
//	IBData.emplace_back(6);
//	IBData.emplace_back(7);
//
//	IBData.emplace_back(3);
//	IBData.emplace_back(4);
//	IBData.emplace_back(1);
//
//	IBData.emplace_back(4);
//	IBData.emplace_back(3);
//	IBData.emplace_back(7);
//
//	IBData.emplace_back(5);
//	IBData.emplace_back(6);
//	IBData.emplace_back(2);
//
//	IBData.emplace_back(5);
//	IBData.emplace_back(2);
//	IBData.emplace_back(0);
//
//	IBData.emplace_back(7);
//	IBData.emplace_back(6);
//	IBData.emplace_back(4);
//
//	IBData.emplace_back(6);
//	IBData.emplace_back(5);
//	IBData.emplace_back(4);
//
//	IBData.emplace_back(0);
//	IBData.emplace_back(1);
//	IBData.emplace_back(4);
//
//	IBData.emplace_back(0);
//	IBData.emplace_back(4);
//	IBData.emplace_back(5);
//
//	// Buffer Description
//	D3D11_BUFFER_DESC VBDesc;
//	VBDesc.Usage = D3D11_USAGE_DEFAULT;
//	VBDesc.ByteWidth = VBData.size() * sizeof(LayoutDefine::Vin1stStatic);
//	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	VBDesc.CPUAccessFlags = 0;
//	VBDesc.MiscFlags = 0;
//
//	// Subresource Data
//	D3D11_SUBRESOURCE_DATA VBInitData;
//	VBInitData.pSysMem = &(VBData[0]);
//	VBInitData.SysMemPitch = 0;
//	VBInitData.SysMemSlicePitch = 0;
//
//	// Create the vertex buffer.
//	HRESULT hr = _DXStorage->_device->CreateBuffer(&VBDesc, &VBInitData, &VB);
//
//	// Buffer Description
//	D3D11_BUFFER_DESC IBDesc;
//	IBDesc.Usage = D3D11_USAGE_DEFAULT;
//	IBDesc.ByteWidth = IBData.size() * sizeof(int);
//	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	IBDesc.CPUAccessFlags = 0;
//	IBDesc.MiscFlags = 0;
//
//	// Subresource Data
//	D3D11_SUBRESOURCE_DATA IBInitData;
//	IBInitData.pSysMem = &(IBData[0]);
//	IBInitData.SysMemPitch = 0;
//	IBInitData.SysMemSlicePitch = 0;
//
//	// Create the Index buffer.
//	hr = _DXStorage->_device->CreateBuffer(&IBDesc, &IBInitData, &IB);
//}
//
//void Pg::Graphics::TestCube::BindBuffers()
//{
//	UINT stride = sizeof(LayoutDefine::Vin1stStatic);
//	UINT offset = 0;
//
//	_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
//	_DXStorage->_deviceContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);
//}
