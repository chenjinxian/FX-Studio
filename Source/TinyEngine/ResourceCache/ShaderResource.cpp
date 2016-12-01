#include "ShaderResource.h"
#include "../AppFramework/BaseGameApp.h"
#include <d3dcompiler.h>

HlslResourceExtraData::HlslResourceExtraData()
{

}

HlslResourceExtraData::~HlslResourceExtraData()
{

}

bool FxEffectResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{
	BaseGameApp::Renderer renderer = g_pApp->GetRendererType();
	if (renderer == BaseGameApp::Renderer_D3D11)
	{
		shared_ptr<HlslResourceExtraData> extra = shared_ptr<HlslResourceExtraData>(DEBUG_NEW HlslResourceExtraData());

		if (g_pApp->GetRendererAPI()->VCompileShaderFromMemory(rawBuffer, rawSize, extra))
		{
			ID3DX11Effect* pEffect = extra->m_pEffect;
			D3DX11_EFFECT_DESC effectDesc;
			if (FAILED(pEffect->GetDesc(&effectDesc)))
			{
				DEBUG_ERROR("ID3DX11Effect::GetDesc() failed.");
			}

			for (uint32_t i = 0; i < effectDesc.Techniques; i++)
			{
				ID3DX11EffectTechnique* pTechnique = pEffect->GetTechniqueByIndex(i);
				D3DX11_TECHNIQUE_DESC techniqueDesc;
				pTechnique->GetDesc(&techniqueDesc);
				for (uint32_t i = 0; i < techniqueDesc.Passes; i++)
				{
				}
			}

			handle->SetExtraData(extra);
			return true;
		}
	}

	DEBUG_ASSERT(0 && "Unsupported Renderer in TextureResourceLoader::VLoadResource");
	return false;
}

bool CsoEffectResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{

}
