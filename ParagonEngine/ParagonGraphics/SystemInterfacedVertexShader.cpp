#include "SystemInterfacedVertexShader.h"
#include "LowDX11Storage.h"
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <cassert>

namespace Pg::Graphics
{

	SystemInterfacedVertexShader::SystemInterfacedVertexShader(const std::wstring& wFilePath, ID3D11InputLayout* tInputLayout, 
		ID3D11RasterizerState* rsState, D3D_PRIMITIVE_TOPOLOGY topology, const std::string& shdVariableName, std::initializer_list<std::string> shdConcreteClassNames)
		: SystemShader(wFilePath), _shader(nullptr), _inputLayout(tInputLayout), _rsState(rsState), _topology(topology), _shdVarName(shdVariableName), _concreteClassNamesVector(shdConcreteClassNames)
	{
		////Class Linkage 오브젝트 만들기.
		//ID3D11ClassLinkage* g_pVSClassLinkage = NULL;
		//HR(_DXStorage->_device->CreateClassLinkage(&g_pVSClassLinkage));
		//
		////Vertex Shader Loading
		//HR(_DXStorage->_device->CreateVertexShader(_byteCode->GetBufferPointer(), _byteCode->GetBufferSize(), g_pVSClassLinkage, &_shader));
		//
		//assert(_inputLayout != nullptr);
		//
		////Interface 파악을 위한 Reflect.
		//ID3D11ShaderReflection* pReflector = NULL;
		//HR(D3DReflect(_byteCode->GetBufferPointer(),
		//	_byteCode->GetBufferSize(),
		//	IID_ID3D11ShaderReflection, (void**)&pReflector));
		//
		////셰이더 내부 인터페이스 개수 파악.
		//_interfacesCount = pReflector->GetNumInterfaceSlots();
		////assert(_concreteClassNamesVector.size() == _interfacesCount); // 같아야.
		//
		//// 인터페이스 인스턴스들을 갖고 있기 충분한 Array를 만든다.
		//_dynamicLinkageArray =
		//	(ID3D11ClassInstance**)malloc(sizeof(ID3D11ClassInstance*) * _interfacesCount);
		//
		//ID3D11ShaderReflectionVariable* pInterfaceVariableName =
		//	pReflector->GetVariableByName(shdVariableName.c_str());
		//
		//D3D11_SHADER_VARIABLE_DESC tDesc;
		//HR(pInterfaceVariableName->GetDesc(&tDesc));
		//assert(pInterfaceVariableName != nullptr);
		//
		////기본적으로 Non-Array Element이니, 0그자체 저장.
		//_interfaceVarOffset = pInterfaceVariableName->GetInterfaceSlot(0);
		//
		//_linkageStorageVector.resize(_concreteClassNamesVector.size());
		//
		//for (int i = 0; i < _concreteClassNamesVector.size(); i++)
		//{
		//	ID3D11ClassInstance* dInstance = NULL;
		//	HR(g_pVSClassLinkage->GetClassInstance(_concreteClassNamesVector.at(i).c_str(), 0, &dInstance));
		//	_linkageStorageVector.at(i) = dInstance;
		//}
	}

	SystemInterfacedVertexShader::~SystemInterfacedVertexShader()
	{
		//free(_dynamicLinkageArray);
	}

	ID3D11VertexShader* SystemInterfacedVertexShader::GetVertexShader()
	{
		return _shader;
	}

	void SystemInterfacedVertexShader::Bind(unsigned int classIndex)
	{
		//// Input Layout
		//_DXStorage->_deviceContext->IASetInputLayout(_inputLayout);
		//// Topology 
		//_DXStorage->_deviceContext->IASetPrimitiveTopology(_topology);
		//// RS.
		//_DXStorage->_deviceContext->RSSetState(_rsState);
		//
		//// Shader - Linkage와 같이 활용해서 바인딩한다.
		//_dynamicLinkageArray[_interfaceVarOffset] = _linkageStorageVector.at(classIndex);
		////_dynamicLinkageArray[instanceIndex] = _linkageStorageVector.at(instanceIndex);
		//_DXStorage->_deviceContext->VSSetShader(_shader, _dynamicLinkageArray, _interfacesCount);
	}

	void SystemInterfacedVertexShader::Unbind()
	{
		//_DXStorage->_deviceContext->VSSetShader(nullptr, nullptr, 0);
	}

}