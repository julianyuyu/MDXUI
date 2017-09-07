#pragma once

#include "orthorenderer.h"
#include "mdxuishader.h"

class MDXUIRenderer : public OrthoRenderer
{
public:
	MDXUIRenderer(ID3D11Device* dev, int w, int h) : OrthoRenderer(dev, w, h)
	{
	}
	virtual ~MDXUIRenderer()
	{
		SAFEDELETE(m_pShader);
	}

	virtual bool Create();
	virtual void Render(int indexCount)
	{
#if USE_LOCAL_RASTER_STATE
		AutoBlendStateHolder bsh(m_pImmeDC);
		AutoDSStateHolder dssh(m_pImmeDC);
		AutoRasterStateHolder rsh(m_pImmeDC, m_pRasterSolidState);
		EnableAlphaBlendState();
		DisableDepthState();
#endif
		m_pShader->Render(indexCount, &m_MatView, &m_MatOrtho, m_pTexList->SRV(0));
	}
private:
	OrthoShader *m_pShader;
};
