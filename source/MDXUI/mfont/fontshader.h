#ifndef _FONTSHADER_H_
#define _FONTSHADER_H_

#pragma once

#include "creator.h"
#include "orthorenderer.h"
#include "XMWrapper.h"

using namespace WrappedXM;

class FontShader : public OrthoShader
{
	struct FONT_CB_COLOR
	{
		WXMVECTOR4 pixelColor;
	};

public:
	FontShader(ID3D11Device* dev) :OrthoShader(dev), m_pCbColor(nullptr) {}
	virtual ~FontShader() {	SAFERELEASE(m_pCbColor); }

	virtual bool Create();
	virtual void Render(UINT idxCount, WXMMATRIX* mat_view, WXMMATRIX* mat_projection, WXMVECTOR4* color, ID3D11ShaderResourceView* tex)
	{
		if (!UpdateShaderResource(mat_view, mat_projection, color)) return;
		m_pImmeDC->PSSetShaderResources(0, 1, &tex);
		DrawIndexed(idxCount);
	}

protected:
	virtual bool CreateResource();
	virtual bool UpdateShaderResource(WXMMATRIX* mat_view, WXMMATRIX* mat_projection, WXMVECTOR4* color);
	ID3D11Buffer *m_pCbColor; // Const buffer for PS color
};

#endif
