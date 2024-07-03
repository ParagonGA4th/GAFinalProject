#include "FadeInOutPass.h"
#include "LowDX11Storage.h"
#include "D3DCarrier.h"
namespace Pg::Graphics
{
	FadeInOutPass::FadeInOutPass() : _DXStorage(LowDX11Storage::GetInstance())
	{
		
	}

	void FadeInOutPass::Initialize()
	{
		_dualPostProcess = std::make_unique<DirectX::DualPostProcess>(_DXStorage->_device);
		CreateBlankTexture(_DXStorage->_device, Pg::Data::GameConstantData::WIDTH, Pg::Data::GameConstantData::HEIGHT);
	}

	void FadeInOutPass::CreateBlankTexture(ID3D11Device* pDevice, float width, float height)
	{
		UINT uWidth = static_cast<UINT>(width);
		UINT uHeight = static_cast<UINT>(height);

		// Create initial data
		std::vector<UINT> initialData(uWidth * uHeight, 0xFF000000); // RGBA: {0,0,0,1}

		// Create texture description
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = uWidth;
		desc.Height = uHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		// Set up the initial data
		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = initialData.data();
		initData.SysMemPitch = uWidth * sizeof(UINT);

		// Create the texture
		HR(pDevice->CreateTexture2D(&desc, &initData, &_blankTexture));

		// Create the shader resource views
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		HR(pDevice->CreateShaderResourceView(_blankTexture, &srvDesc, &_blankSRV));
	}

	void FadeInOutPass::Render(float baseShowRatio, D3DCarrier* d3dCarrier, ID3D11ShaderResourceView* sourceTexture)
	{
		//전에 이미 RenderTarget은 셋되어 있어야 한다.
		_dualPostProcess->SetEffect(DirectX::DualPostProcess::Merge);
		_dualPostProcess->SetSourceTexture(sourceTexture);
		_dualPostProcess->SetSourceTexture2(_blankSRV);

		_dualPostProcess->SetMergeParameters(baseShowRatio, 1.f - baseShowRatio);
		_dualPostProcess->Process(_DXStorage->_deviceContext);
	}



}
