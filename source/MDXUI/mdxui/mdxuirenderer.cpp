#include "inc.h"

#include "orthorenderer.h"
#include "mdxuirenderer.h"
#include "mdxuitex.h"
#include "mdxuiTexData.h"

bool MDXUIRenderer::Create()
{
	//	m_pImmeDC->RSSetState(m_pRasterStateSolid);

	m_pTexList = new MUTTexList(m_pDevice);
	//m_pTexList->Append(MDXUI_TEXTURE_FILENAME, false);
	m_pTexList->AppendFromMemory((void *)MDXUITextureData, ARRAYSIZE(MDXUITextureData)*4, false);

//	InitMatrix(w, h);
	m_pShader = new OrthoShader(m_pDevice);
	if (m_pShader)
		return m_pShader->Create(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

	return false;
}
