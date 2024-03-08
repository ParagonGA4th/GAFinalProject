#include "AssetCombinedLoader.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "DX11Headers.h"
#include "MaterialParser.h"
#include "AnimationParser.h"

//Made Resources
#include "RenderVertexShader.h"
#include "RenderPixelShader.h"
#include "RenderMaterial.h"
#include "RenderAnimation.h"

namespace Pg::Graphics::Loader
{
	AssetCombinedLoader::AssetCombinedLoader() : _DXStorage(LowDX11Storage::GetInstance())
	{
		_matParser = std::make_unique<Pg::Graphics::MaterialParser>();
		_animParser = std::make_unique<Pg::Graphics::AnimationParser>();
	}

	AssetCombinedLoader::~AssetCombinedLoader()
	{

	}

	void AssetCombinedLoader::LoadRenderVertexShader(const std::string& path, RenderVertexShader* renderVS)
	{
		std::wstring wFilePath;
		wFilePath.assign(renderVS->_filePath.begin(), renderVS->_filePath.end());

		//Blob으로 D3D 파일을 읽어온다. (Load)
		HR(D3DReadFileToBlob(wFilePath.c_str(), &(renderVS->_byteCode)));

		//Vertex Shader 갖고 오기.
		HR(_DXStorage->_device->CreateVertexShader(renderVS->_byteCode->GetBufferPointer(), 
			renderVS->_byteCode->GetBufferSize(), NULL, &(renderVS->_shader)));

		//InputLayout : 정한 기본으로 갖고 오기.
		renderVS->_inputLayout = LayoutDefine::GetDeferredQuadLayout();
	}

	void AssetCombinedLoader::LoadRenderPixelShader(const std::string& path, RenderPixelShader* renderPS)
	{
		std::wstring wFilePath;
		wFilePath.assign(renderPS->_filePath.begin(), renderPS->_filePath.end());

		//Blob으로 D3D 파일을 읽어온다. (Load)
		HR(D3DReadFileToBlob(wFilePath.c_str(), &(renderPS->_byteCode)));

		//Vertex Shader 갖고 오기.
		HR(_DXStorage->_device->CreatePixelShader(renderPS->_byteCode->GetBufferPointer(),
			renderPS->_byteCode->GetBufferSize(), NULL, &(renderPS->_shader)));
	}

	void AssetCombinedLoader::LoadCustomRenderMaterial(const std::string& path, RenderMaterial* renderMat)
	{
		_matParser->LoadCustomRenderMaterial(path, renderMat);
	}

	void AssetCombinedLoader::LoadDefaultRenderMaterial(const std::string& defInstMatName, RenderMaterial* renderMat)
	{
		_matParser->LoadDefaultRenderMaterialInstance(defInstMatName, renderMat);
	}

	void AssetCombinedLoader::LoadAnimation(const std::string& path, RenderAnimation* anim)
	{
		_animParser->ParseAnimation(path, anim);
		_animParser->Clean();
	}

}