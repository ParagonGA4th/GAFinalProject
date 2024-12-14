#pragma once
#include "SystemShader.h"
#include <vector>
#include <initializer_list>

namespace Pg::Graphics
{
	class SystemInterfacedVertexShader : public SystemShader
	{
	public:
		SystemInterfacedVertexShader(const std::wstring& wFilePath, ID3D11InputLayout* tInputLayout, ID3D11RasterizerState* rsState,
			D3D_PRIMITIVE_TOPOLOGY topology, const std::string& shdVariableName, std::initializer_list<std::string> shdConcreteClassNames);
		virtual ~SystemInterfacedVertexShader();

		ID3D11VertexShader* GetVertexShader();
		void Bind(unsigned int classIndex);
		void Unbind();
	private:
		ID3D11VertexShader* _shader;
		ID3D11InputLayout* _inputLayout;
		D3D_PRIMITIVE_TOPOLOGY _topology;
		ID3D11RasterizerState* _rsState;

	private:
		std::string _shdVarName;
		unsigned int _interfacesCount;
		unsigned int _interfaceVarOffset;
		ID3D11ClassInstance** _dynamicLinkageArray{ nullptr }; // ���ε���.
		std::vector<std::string> _concreteClassNamesVector; //Index�� ����� �� ����.
		std::vector<ID3D11ClassInstance*> _linkageStorageVector; //�� String Vector�� �ε��� ��Ī.
	};
}


